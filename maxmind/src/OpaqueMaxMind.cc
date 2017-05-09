#include "Plugin.h"
#include "OpaqueMaxMind.h"

using namespace plugin::Bro_MaxMind;

OpaqueType* maxminddb_type = new OpaqueType("maxminddb");
OpaqueType* maxmindresult_type = new OpaqueType("maxmind_result");

MMDBVal::MMDBVal() : OpaqueVal(new OpaqueType("maxminddb"))
	{
	}

MMDBVal::~MMDBVal()
	{
	MMDB_close(&this->mmdb);
	}

//IMPLEMENT_SERIAL(MMDBVal, SER_MMDB_VAL);
//bool MMDBVal::DoSerialize(SerialInfo* info) const
//	{
//	//DO_SERIALIZE(SER_MMDB_VAL, OpaqueVal);
//	return true;
//	}
//
//bool MMDBVal::DoUnserialize(UnserialInfo* info)
//	{
//	//DO_UNSERIALIZE(OpaqueVal);
//	return true;
//	}


MaxMindResultVal::MaxMindResultVal(MMDB_lookup_result_s arg_result): OpaqueVal(new OpaqueType("maxmind_result"))
	{
	result = arg_result;
	}

MaxMindResultVal::~MaxMindResultVal()
	{
	}