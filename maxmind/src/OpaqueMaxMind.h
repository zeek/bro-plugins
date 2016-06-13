#ifndef OPAQUEMAXMIND_H
#define OPAQUEMAXMIND_H

#include "OpaqueVal.h"
#include "maxminddb.h"

namespace plugin {
namespace Bro_MaxMind {

static OpaqueType* maxminddb_type;
static OpaqueType* maxmindresult_type;

class MMDBVal : public OpaqueVal {
public:
	MMDBVal();

	MMDB_s mmdb;

protected:
	friend class Val;
	MMDBVal(OpaqueType* t);

	//DECLARE_SERIAL(MMDBVal);
private:
	~MMDBVal();
};

class MaxMindResultVal : public OpaqueVal {
public:
	MaxMindResultVal(MMDB_lookup_result_s arg_result);

	MMDB_s *mmdb;
	MMDB_lookup_result_s result;

protected:
	friend class Val;
	MaxMindResultVal(OpaqueType* t);
	//DECLARE_SERIAL(MaxMindResultVal);

private:
	~MaxMindResultVal();
};

}
}

#endif