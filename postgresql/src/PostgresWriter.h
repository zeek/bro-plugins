// See the file "COPYING" in the main distribution directory for copyright.
//
// Log writer for POSTGRES logs.

#ifndef LOGGING_WRITER_POSTGRES_H
#define LOGGING_WRITER_POSTGRES_H

#include <thread>

#include "logging/WriterBackend.h"
#include "threading/formatters/Ascii.h"
#include "libpq-fe.h"

namespace logging { namespace writer {

class PostgreSQL : public WriterBackend {
public:
	PostgreSQL(WriterFrontend* frontend);
	~PostgreSQL();

	// prohibit copying and moving
	PostgreSQL(const PostgreSQL&) = delete;
	PostgreSQL& operator=(const PostgreSQL&) = delete;
	PostgreSQL(PostgreSQL&&) = delete;

	static WriterBackend* Instantiate(WriterFrontend* frontend)
		{ return new PostgreSQL(frontend); }

protected:
	bool DoInit(const WriterInfo& info, int num_fields, const threading::Field* const* fields) override;
	bool DoWrite(int num_fields, const threading::Field* const* fields, threading::Value** vals) override;
	bool DoSetBuf(bool enabled) override;
	bool DoRotate(const char* rotated_path, double open, double close, bool terminating) override;
	bool DoFlush(double network_time) override;
	bool DoFinish(double network_time) override;
	bool DoHeartbeat(double network_time, double current_time) override;

private:
	string LookupParam(const WriterInfo& info, const string name) const;
	std::tuple<bool, string> CreateParams(const threading::Value* val);
	string GetTableType(int, int);
	void CreateInsert(int num_fields, const threading::Field* const* fields);

	PGconn *conn;

	string table;
	string insert;

	string default_hostname;
	string default_dbname;
	int default_port;

	std::unique_ptr<threading::formatter::Ascii> io;
};

}
}

#endif /* LOGGING_WRITER_POSTGRES_H */
