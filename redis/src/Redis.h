// See the file "COPYING" in the main distribution directory for copyright.
//
// Redis log writer that just discards everything (but still pretends to
// rotate).

#ifndef LOGGING_WRITER_REDIS_H
#define LOGGING_WRITER_REDIS_H

#include "logging/WriterBackend.h"
#include "threading/Formatter.h"
#include "util.h"

#include <hiredis.h>

namespace logging { namespace writer {

class Redis : public WriterBackend {
public:
	Redis(WriterFrontend* frontend);
	~Redis();

	static WriterBackend* Instantiate(WriterFrontend* frontend)
		{ return new Redis(frontend); }

protected:
	virtual bool DoInit(const WriterInfo& info, int num_fields,
			    const threading::Field* const * fields);
	virtual bool DoWrite(int num_fields, const threading::Field* const* fields,
			     threading::Value** vals);
	virtual bool DoSetBuf(bool enabled);
	virtual bool DoRotate(const char* rotated_path, double open,
			      double close, bool terminating);
	virtual bool DoFlush(double network_time);
	virtual bool DoFinish(double network_time);
	virtual bool DoHeartbeat(double network_time, double current_time);

private:
	bool InitFilterOptions(const WriterInfo& info, int num_fields, const threading::Field* const * fields);
	bool InitConfigOptions();
	bool InitFormatters();
	bool RedisConnect();
	uint64 SendRedisBatch(uint nretries = 3);

	std::string hostname;
	std::string unix_path;
	uint64 port;
	uint64 db;

	uint64 key_index;
	std::string key_prefix;
	std::string key;

	std::string json_timestamps;
	threading::formatter::Formatter* key_formatter;
	threading::formatter::Formatter* val_formatter;
	redisContext *rcontext;
	bool buffered;
	uint64 enqueued_cntr;

	double last_flush_ts;
	double flush_period;
	uint64 key_expire;

	ODesc key_desc;
	ODesc val_desc;
};

}
}

#endif
