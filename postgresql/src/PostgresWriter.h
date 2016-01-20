// See the file "COPYING" in the main distribution directory for copyright.
//
// Log writer for POSTGRES logs.

#ifndef LOGGING_WRITER_POSTGRES_H
#define LOGGING_WRITER_POSTGRES_H

#include "logging/WriterBackend.h"
#include "threading/formatters/Ascii.h"
#include "libpq-fe.h"

namespace logging { namespace writer {

class PostgreSQL : public WriterBackend {
public:
	PostgreSQL(WriterFrontend* frontend);
	~PostgreSQL();

	static WriterBackend* Instantiate(WriterFrontend* frontend)
		{ return new PostgreSQL(frontend); }

protected:
	virtual bool DoInit(const WriterInfo& info, int num_fields,
	const threading::Field* const* fields);
	virtual bool DoWrite(int num_fields, const threading::Field* const* fields, threading::Value** vals);
	virtual bool DoSetBuf(bool enabled);
	virtual bool DoRotate(const char* rotated_path, double open, double close, bool terminating);
	virtual bool DoFlush(double network_time);
	virtual bool DoFinish(double network_time);
	virtual bool DoHeartbeat(double network_time, double current_time);

private:
	int AddParams(threading::Value* val, vector<char*> &params, string &call, int currId, bool addcomma);
	string GetTableType(int, int);
	char* FS(const char* format, ...);

	PGconn *conn;

	string table;

	threading::formatter::Ascii* io;
};

}
}

#endif /* LOGGING_WRITER_POSTGRES_H */
