// See the file "COPYING" in the main distribution directory for copyright.

#include <string>
#include <errno.h>
#include <vector>

#include "bro-config.h"

#include "NetVar.h"
#include "threading/SerialTypes.h"

#include "PostgresWriter.h"

using namespace logging;
using namespace writer;
using threading::Value;
using threading::Field;

PostgreSQL::PostgreSQL(WriterFrontend* frontend) : WriterBackend(frontend)
	{
	io = unique_ptr<threading::formatter::Ascii>(new threading::formatter::Ascii(this, threading::formatter::Ascii::SeparatorInfo()));
	}

PostgreSQL::~PostgreSQL()
	{
	if ( conn != 0 )
		PQfinish(conn);
	}

string PostgreSQL::GetTableType(int arg_type, int arg_subtype)
	{
	string type;

	switch ( arg_type ) {
	case TYPE_BOOL:
		type = "boolean";
		break;

	case TYPE_INT:
	case TYPE_COUNT:
	case TYPE_COUNTER:
	case TYPE_PORT:
		type = "integer";
		break;

	/*
	case TYPE_PORT:
		type = "VARCHAR(10)";
		break; */

	case TYPE_SUBNET:
	case TYPE_ADDR:
		type = "inet";
		break;

	case TYPE_TIME:
	case TYPE_INTERVAL:
	case TYPE_DOUBLE:
		type = "double precision";
		break;

	case TYPE_ENUM:
	case TYPE_STRING:
	case TYPE_FILE:
	case TYPE_FUNC:
		type = "TEXT";
		break;

	case TYPE_TABLE:
	case TYPE_VECTOR:
		type = GetTableType(arg_subtype, 0) + "[]";
		break;

	default:
		Error(Fmt("unsupported field format %d ", arg_type));
		return string();
	}

	return type;
}

// preformat the insert string that we only need to create once during our lifetime
void PostgreSQL::CreateInsert(int num_fields, const Field* const * fields)
	{
	string names = "INSERT INTO "+table+" ( ";
	string values("VALUES (");

	for ( int i = 0; i < num_fields; ++i )
		{
		string fieldname = fields[i]->name;
		replace( fieldname.begin(), fieldname.end(), '.', '_' ); // postgres does not like "." in row names.

		if ( i != 0 )
			{
			values += ", ";
			names += ", ";
			}

		names += fieldname;
		values += "$" + std::to_string(i+1);
		}

	insert = names + ") " + values + ");";
	}

string PostgreSQL::LookupParam(const WriterInfo& info, const string name) const
	{
	map<const char*, const char*>::const_iterator it = info.config.find(name.c_str());
	if ( it == info.config.end() )
		return string();
	else
		return it->second;
	}

bool PostgreSQL::DoInit(const WriterInfo& info, int num_fields,
			    const Field* const * fields)
	{
	table = info.path;
	string conninfo = LookupParam(info, "conninfo");
	if ( conninfo.empty() )
		{
		string hostname = LookupParam(info, "hostname");
		if ( hostname.empty() )
			{
			MsgThread::Info(Fmt("hostname configuration option not found. Defaulting to localhost"));
			hostname = "localhost";
			}

		string dbname = LookupParam(info, "dbname");
		if ( dbname.empty() )
			{
			MsgThread::Error(Fmt("dbname configuration option not found."));
			return false;
			}

		conninfo = string("host = ") + hostname + " dbname = " + dbname;

		string port = LookupParam(info, "port");
		if ( ! port.empty() )
			conninfo += " port = " + port;
		}

	conn = PQconnectdb(conninfo.c_str());

	if ( PQstatus(conn) != CONNECTION_OK )
		{
		Error(Fmt("Could not connect to pg (%s): %s", conninfo.c_str(), PQerrorMessage(conn)));
		return false;
		}

	string create = "CREATE TABLE IF NOT EXISTS "+table+" (\n"
		"id SERIAL UNIQUE NOT NULL";

	for ( int i = 0; i < num_fields; ++i )
		{
		const Field* field = fields[i];

		create += ",\n";

		string name = field->name;
		replace( name.begin(), name.end(), '.', '_' ); // postgres does not like "." in row names.
		create += name;

		string type = GetTableType(field->type, field->subtype);

		create += " "+type;
		/* if ( !field->optional ) {
			create += " NOT NULL";
		} */
		}

	create += "\n);";

	PGresult *res = PQexec(conn, create.c_str());
	if ( PQresultStatus(res) != PGRES_COMMAND_OK)
		{
		Error(Fmt("Create command failed: %s\n", PQerrorMessage(conn)));
		return false;
		}

	CreateInsert(num_fields, fields);

	return true;
	}

bool PostgreSQL::DoFlush(double network_time)
	{
	return true;
	}

bool PostgreSQL::DoFinish(double network_time)
	{
	return true;
	}

bool PostgreSQL::DoHeartbeat(double network_time, double current_time)
	{
	return true;
	}

std::tuple<bool, string> PostgreSQL::CreateParams(const Value* val)
	{
	if ( ! val->present )
		return std::make_tuple(false, string());

	string retval;

	switch ( val->type ) {

	case TYPE_BOOL:
		retval = val->val.int_val ? "T" : "F";
		break;

	case TYPE_INT:
		retval = std::to_string(val->val.int_val);
		break;

	case TYPE_COUNT:
	case TYPE_COUNTER:
		retval = std::to_string(val->val.uint_val);
		break;

	case TYPE_PORT:
		retval = std::to_string(val->val.port_val.port);
		break;

	case TYPE_SUBNET:
		retval = io->Render(val->val.subnet_val);
		break;

	case TYPE_ADDR:
		retval = io->Render(val->val.addr_val);
		break;

	case TYPE_TIME:
	case TYPE_INTERVAL:
	case TYPE_DOUBLE:
		retval = std::to_string(val->val.double_val);
		break;

	case TYPE_ENUM:
	case TYPE_STRING:
	case TYPE_FILE:
	case TYPE_FUNC:
		if ( ! val->val.string_val.length || val->val.string_val.length == 0 )
			return std::make_tuple(false, string());

		retval = string(val->val.string_val.data, val->val.string_val.length);
		break;

	case TYPE_TABLE:
	case TYPE_VECTOR:
		{
		bro_int_t size;
		Value** vals;

		string out("{");

		if ( val->type == TYPE_TABLE )
			{
			size = val->val.set_val.size;
			vals = val->val.set_val.vals;
			}
		else
			{
			size = val->val.vector_val.size;
			vals = val->val.vector_val.vals;
			}

		if ( ! size )
			return std::make_tuple(false, string());

		for ( int i = 0; i < size; ++i )
			{
			if ( i != 0 )
				out += ", ";

			auto res = CreateParams(vals[i]);
			if ( std::get<0>(res) == false )
				{
				out += "NULL";
				continue;
				}

			string resstr = std::get<1>(res);
			TypeTag type = vals[i]->type;
			// for all numeric types, we do not need escaping
			if ( type == TYPE_BOOL || type == TYPE_INT || type == TYPE_COUNT ||
					type == TYPE_COUNTER || type == TYPE_PORT || type == TYPE_TIME ||
					type == TYPE_INTERVAL || type == TYPE_DOUBLE )
				out += resstr;
			else
				{
				char* escaped = PQescapeLiteral(conn, resstr.c_str(), resstr.size());
				if ( escaped == nullptr )
					{
					Error(Fmt("Error while escaping '%s'", resstr.c_str()));
					return std::make_tuple(false, string());
					}
				else
					{
					out += escaped;
					PQfreemem(escaped);
					}
				}
			}

		out += "}";
		retval = out;
		break;
		}

	default:
		Error(Fmt("unsupported field format %d", val->type ));
		return std::make_tuple(false, string());
	}

	return std::make_tuple(true, retval);
	}

bool PostgreSQL::DoWrite(int num_fields, const Field* const* fields, Value** vals)
	{
	printf("Call: %s\n", insert.c_str());
	vector<std::tuple<bool, string>> params; // vector in which we compile the string representation of characters

	for ( int i = 0; i < num_fields; ++i )
		params.push_back(CreateParams(vals[i]));

	vector<const char*> params_char; // vector in which we compile the character pointers that we
	// then pass to PQexecParams. These do not have to be cleaned up because the srings will be
	// cleaned up automatically.

	for ( auto &i: params )
		{
		if ( std::get<0>(i) == false)
			params_char.push_back(nullptr); // null pointer is accepted to signify NULL in parameters
		else
			params_char.push_back(std::get<1>(i).c_str());
		}

	assert( params_char.size() == num_fields );

	// & of vector is legal - according to current STL standard, vector has to be saved in consecutive memory.
	PGresult *res = PQexecParams(conn,
			insert.c_str(),
			params_char.size(),
			NULL,
			&params_char[0],
			NULL,
			NULL,
			0);

	if ( PQresultStatus(res) != PGRES_COMMAND_OK)
		{
		printf("Command failed: %s\n", PQerrorMessage(conn));
		Error(Fmt("Command failed: %s\n", PQerrorMessage(conn)));
		PQclear(res);
		return false;
		}

	PQclear(res);
	return true;
	}

bool PostgreSQL::DoRotate(const char* rotated_path, double open, double close, bool terminating)
	{
	return true;
	}

bool PostgreSQL::DoSetBuf(bool enabled)
	{
	// Nothing to do.
	return true;
	}
