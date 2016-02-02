// See the file "COPYING" in the main distribution directory for copyright.

#include <fstream>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "bro-config.h"

#include "NetVar.h"
#include "threading/SerialTypes.h"

#include "PostgresReader.h"

using namespace input::reader;
using threading::Value;
using threading::Field;


PostgreSQL::PostgreSQL(ReaderFrontend *frontend) : ReaderBackend(frontend)
	{
	io = unique_ptr<threading::formatter::Ascii>(new threading::formatter::Ascii(this, threading::formatter::Ascii::SeparatorInfo()));
	}

PostgreSQL::~PostgreSQL()
	{
	if ( conn != 0 )
		PQfinish(conn);
	DoClose();
	}

void PostgreSQL::DoClose()
	{
	}

string PostgreSQL::LookupParam(const ReaderInfo& info, const string name) const
	{
	map<const char*, const char*>::const_iterator it = info.config.find(name.c_str());
	if ( it == info.config.end() )
		return string();
	else
		return it->second;
	}

bool PostgreSQL::DoInit(const ReaderInfo& info, int arg_num_fields, const threading::Field* const* arg_fields)
	{
	assert(arg_fields);
	assert(arg_num_fields >= 0);

	string conninfo = LookupParam(info, "conninfo");
	if ( conninfo.empty() )
		{
		string hostname = LookupParam(info, "hostname");
		if ( hostname.empty() )
			{
			MsgThread::Info("hostname configuration option not found. Defaulting to localhost.");
			hostname = "localhost";
			}

		string dbname = LookupParam(info, "dbname");
		if ( dbname.empty() )
			{
			Error("dbname configuration option not found. Aborting.");
			return false;
			}

		conninfo = string("host = ") + hostname + " dbname = " + dbname;

		string port = LookupParam(info, "port");
		if ( ! port.empty() )
			conninfo += " port = " + port;
		}

	conn = PQconnectdb(conninfo.c_str());

	num_fields = arg_num_fields;
	fields = arg_fields;

	if ( PQstatus(conn) != CONNECTION_OK )
		{
		Error(Fmt("Could not connect to pg (%s): %s", conninfo.c_str(), PQerrorMessage(conn)));
		return false;
		}

	query = info.source;

	DoUpdate();

	return true;
	}

std::unique_ptr<Value> PostgreSQL::EntryToVal(string s, const threading::Field* field)
	{
	std::unique_ptr<Value> val(new Value(field->type, true));

	switch ( field->type ) {
	case TYPE_ENUM:
	case TYPE_STRING:
		val->val.string_val.length  = s.size();
		val->val.string_val.data = copy_string(s.c_str());
		break;

	case TYPE_BOOL:
		if ( s == "t" ) {
			val->val.int_val = 1;
		} else if ( s == "f" ) {
			val->val.int_val = 0;
		} else {
			Error(Fmt("Invalid value for boolean: %s", s.c_str()));
			return nullptr;
		}
		break;

	case TYPE_INT:
		val->val.int_val = atoi(s.c_str());
		break;

	case TYPE_DOUBLE:
	case TYPE_TIME:
	case TYPE_INTERVAL:
		val->val.double_val = atof(s.c_str());
		break;

	case TYPE_COUNT:
	case TYPE_COUNTER:
		val->val.uint_val = atoi(s.c_str());
		break;

	case TYPE_PORT:
		val->val.port_val.port = atoi(s.c_str());
		val->val.port_val.proto = TRANSPORT_UNKNOWN;
		break;

	case TYPE_SUBNET: {
		int pos = s.find("/");
		int width = atoi(s.substr(pos+1).c_str());
		string addr = s.substr(0, pos);

		val->val.subnet_val.prefix = io->ParseAddr(addr);
		val->val.subnet_val.length = width;
		break;

		}
	case TYPE_ADDR:
		val->val.addr_val = io->ParseAddr(s);
		break;

	case TYPE_TABLE:
	case TYPE_VECTOR:
		// First - common stuff
		// Then - initialization for table.
		// Then - initialization for vector.
		{
		istringstream splitstream(s);

		unique_ptr<Field> newfield(new Field(*field));
		newfield->type = field->subtype;

		std::vector<std::unique_ptr<Value>> vals;

		while ( splitstream )
			{
			string element;

			if ( !getline(splitstream, element, ',') )
				break;

			auto newval = EntryToVal(element, newfield.get());
			if ( newval == nullptr ) {
				Error("Error while reading set");
				return nullptr;
			}

			vals.push_back(std::move(newval));
			}


		// this should not leak in case of error -- instead, Value::~Value will clean it up.
		Value** lvals = new Value* [vals.size()];
		for ( int i = 0; i<vals.size(); ++i )
			lvals[i] = vals[i].release();

		if ( field->type == TYPE_TABLE )
			{
			val->val.set_val.vals = lvals;
			val->val.set_val.size = vals.size();
			}
		else if ( field->type == TYPE_VECTOR )
			{
			val->val.vector_val.vals = lvals;
			val->val.vector_val.size = vals.size();
			}
		else
			assert(false);

		break;
		}

	default:
		Error(Fmt("unsupported field format %d for %s", field->type, field->name));
		return 0;
		}

	return val;

	}

bool PostgreSQL::DoUpdate()
	{
	PGresult *res = PQexecParams(conn, query.c_str(), 0, NULL, NULL, NULL, NULL, 0);
	if (PQresultStatus(res) != PGRES_TUPLES_OK)
		{
		Error(Fmt("PostgreSQL query failed: %s", PQerrorMessage(conn)));
		PQclear(res);
		return false;
		}

	std::vector<int> mapping;
	mapping.reserve(num_fields);

	for ( int i = 0; i < num_fields; ++i ) {
		int pos = PQfnumber(res, fields[i]->name);
		if ( pos == -1 )
			{
			Error(Fmt("Field %s was not found in PostgreSQL result", fields[i]->name));
			PQclear(res);
			return false;
			}

		mapping.push_back(pos);
	}

	assert( mapping.size() == num_fields );

	for ( int i = 0; i < PQntuples(res); ++i )
		{
		std::vector<std::unique_ptr<Value>> ovals;
		//ovals.resize(num_fields);

		for ( int j = 0; j < num_fields; ++j )
			{
			if ( PQgetisnull(res, i, mapping[j] ) == 1 )
				ovals.emplace_back(std::unique_ptr<Value>(new Value(fields[j]->type, false)));
			else
				{
				// str will be cleaned up by PQclear.
				char *str = PQgetvalue(res, i, mapping[j]);
				auto res = EntryToVal(str, fields[j]);
				if ( res == nullptr )
					{
					// error occured, let's break out of this line. Just removing ovals will get rid of everything.
					ovals.clear();
					break;
					}

				ovals.push_back(std::move(res));
				}
			}

		// if there is an result, send it on :)
		if ( ! ovals.empty() )
			{
			assert( ovals.size() == num_fields );
			Value** ofields = new Value*[num_fields];

			for ( int i = 0; i < num_fields; ++i )
				{
				ofields[i] = ovals[i].release();
				}

			SendEntry(ofields);
			}
		}

	PQclear(res);
	EndCurrentSend();

	return true;
	}

// currently we do not support streaming
bool PostgreSQL::DoHeartbeat(double network_time, double current_time) 
	{
	return true;
	}

