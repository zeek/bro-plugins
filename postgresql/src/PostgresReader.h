// See the file "COPYING" in the main distribution directory for copyright.

#ifndef INPUT_READERS_POSTGRES_H
#define INPUT_READERS_POSTGRES_H

#include <iostream>
#include <vector>
#include <thread>

#include "input/ReaderFrontend.h"
#include "threading/formatters/Ascii.h"
#include "libpq-fe.h"

namespace input { namespace reader {

class PostgreSQL : public ReaderBackend {
public:
	explicit PostgreSQL(ReaderFrontend* frontend);
	~PostgreSQL();

	// prohibit copying and moving
	PostgreSQL(const PostgreSQL&) = delete;
	PostgreSQL& operator=(const PostgreSQL&) = delete;
	PostgreSQL(PostgreSQL&&) = delete;

	static ReaderBackend* Instantiate(ReaderFrontend* frontend) { return new PostgreSQL(frontend); }

protected:
	bool DoInit(const ReaderInfo& info, int arg_num_fields, const threading::Field* const* fields) override;

	void DoClose() override;

	bool DoUpdate() override;

	bool DoHeartbeat(double network_time, double current_time) override;

private:

	string LookupParam(const ReaderInfo& info, const string name) const;
	std::unique_ptr<threading::Value> EntryToVal(string s, const threading::Field* type);

	PGconn *conn;
	std::unique_ptr<threading::formatter::Ascii> io;

	const threading::Field* const * fields; // raw mapping
	string query;
	int num_fields;
};


}
}

#endif /* INPUT_READERS_POSTGRES_H */
