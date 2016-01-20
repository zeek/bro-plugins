// See the file "COPYING" in the main distribution directory for copyright.

#ifndef INPUT_READERS_POSTGRES_H
#define INPUT_READERS_POSTGRES_H

#include <iostream>
#include <vector>

#include "input/ReaderFrontend.h"
#include "threading/formatters/Ascii.h"
#include "libpq-fe.h"

namespace input { namespace reader {

class PostgreSQL : public ReaderBackend {
public:
	PostgreSQL(ReaderFrontend* frontend);
	~PostgreSQL();

	static ReaderBackend* Instantiate(ReaderFrontend* frontend) { return new PostgreSQL(frontend); }

protected:
	virtual bool DoInit(const ReaderInfo& info, int arg_num_fields, const threading::Field* const* fields);

	virtual void DoClose();

	virtual bool DoUpdate();
	virtual bool DoHeartbeat(double network_time, double current_time);

private:

	unsigned int num_fields;

	const threading::Field* const * fields; // raw mapping

	threading::Value* EntryToVal(string s, const threading::Field *type);

	int mode;

	bool started;
	string query;

	PGconn *conn;

	threading::formatter::Ascii* io;
};


}
}

#endif /* INPUT_READERS_POSTGRES_H */
