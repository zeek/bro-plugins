#include "Redis.h"
#include "redis.bif.h"
#include "threading/MsgThread.h"
#include "threading/formatters/JSON.h"
#include "threading/formatters/Ascii.h"

#include <stdlib.h>

using namespace logging;
using namespace writer;
using namespace threading;

bool Redis::InitFormatters()
	{
	delete key_formatter;
	delete val_formatter;
	formatter::JSON::TimeFormat tf = formatter::JSON::TS_EPOCH;
	if ( strcmp(json_timestamps.c_str(), "JSON::TS_EPOCH") == 0 )
		tf = formatter::JSON::TS_EPOCH;
	else if ( strcmp(json_timestamps.c_str(), "JSON::TS_MILLIS") == 0 )
		tf = formatter::JSON::TS_MILLIS;
	else if ( strcmp(json_timestamps.c_str(), "JSON::TS_ISO8601") == 0 )
		tf = formatter::JSON::TS_ISO8601;
	else
		{
		Error(Fmt("Invalid JSON timestamp format: %s", json_timestamps.c_str()));
		return false;
		}

	val_formatter = new formatter::JSON(this, tf);
	key_formatter = new formatter::Ascii(this, formatter::Ascii::SeparatorInfo());

	return true;
	}

bool Redis::InitFilterOptions(const WriterInfo& info, int num_fields,
		const threading::Field* const * fields)
	{
	for ( WriterInfo::config_map::const_iterator i = info.config.begin();
		i != info.config.end(); ++i )
		{
		if ( strcmp(i->first, "db") == 0 )
			db = strtoul(i->second, NULL, 0);
		else if ( strcmp(i->first, "key_index") == 0 )
			key_index = strtoul(i->second, NULL, 0);
		else if ( strcmp(i->first, "key") == 0 )
			{
			key.assign(i->second);
			// overwrites key_index
			for ( int j = 0; j < num_fields; j++ )
				{
				const threading::Field* field = fields[j];
				if ( strcmp(field->name, i->second) == 0 )
					{
					MsgThread::Info("Key field name setting, overwrites key_index");
					key_index = j;
					break;
					}
				}
			}
		else if ( strcmp(i->first, "server_host") == 0 )
			hostname.assign(i->second);
		else if ( strcmp(i->first, "unix_path") == 0 )
			unix_path.assign(i->second);
		else if ( strcmp(i->first, "server_port") == 0 )
			port = strtoul(i->second, NULL, 0);
		else if ( strcmp(i->first, "key_prefix") == 0 )
			key_prefix.assign(i->second);
		else if ( strcmp(i->first, "json_timestamps") == 0 )
			json_timestamps.assign(i->second);
		else if ( strcmp(i->first, "flush_period") == 0 )
			flush_period = strtod(i->second, NULL);
		else if ( strcmp(i->first, "key_expire") == 0 )
			key_expire = strtoul(i->second, NULL, 0);
		}
	if ( ! InitFormatters() )
		return false;
	return true;
	}

bool Redis::InitConfigOptions()
	{
	hostname.assign(
		(const char*) BifConst::Redis::server_host->Bytes(),
		BifConst::Redis::server_host->Len()
		);
	unix_path.assign(
		(const char*) BifConst::Redis::unix_path->Bytes(),
		BifConst::Redis::unix_path->Len()
		);
	port = BifConst::Redis::server_port;
	db = BifConst::Redis::db;
	key_index = BifConst::Redis::key_index;
	key_prefix.assign(
		(const char*) BifConst::Redis::key_prefix->Bytes(),
		BifConst::Redis::key_prefix->Len()
		);
	ODesc tsfmt;
	BifConst::Redis::json_timestamps->Describe(&tsfmt);
	json_timestamps.assign(
		(const char*) tsfmt.Bytes(),
		tsfmt.Len()
		);
	flush_period = BifConst::Redis::flush_period;
	key_expire = BifConst::Redis::key_expire;

	return true;
	}

Redis::Redis(WriterFrontend* frontend) : WriterBackend(frontend),
	hostname("127.0.0.1"), unix_path(""), port(6379), db(0), key_index(0),
	key_prefix("BRO::"), key(""), json_timestamps("JSON::TS_EPOCH"),
	key_formatter(NULL), val_formatter(NULL), rcontext(NULL),
	buffered(true), enqueued_cntr(0), last_flush_ts(0), flush_period(1000),
	key_expire(0)
	{
	InitConfigOptions();
	}

Redis::~Redis()
	{
	delete key_formatter;
	delete val_formatter;
	}

bool Redis::RedisConnect()
	{
	redisReply *reply;

	struct timeval timeout = { 1, 500000 }; // 1.5 seconds
	if ( unix_path.empty() )
		rcontext = redisConnectWithTimeout(hostname.c_str(), port, timeout);
	else
		rcontext = redisConnectUnixWithTimeout(unix_path.c_str(), timeout);

	if (rcontext == NULL || rcontext->err)
		{
		if (rcontext)
			{
			Error(rcontext->errstr);
			redisFree(rcontext);
			}
		else
			{
			Error("Connection error: can't allocate redis context\n");
			}
		return false;
		}

	reply = reinterpret_cast<redisReply*>(redisCommand(rcontext, "SELECT %d", db));
	if (reply == NULL)
		{
		Error(rcontext->errstr);
		return false;
		}

	freeReplyObject(reply);

	return true;
	}

bool Redis::DoInit(const WriterInfo& info, int num_fields,
		const threading::Field* const * fields)
	{
	InitFilterOptions(info, num_fields, fields);

	if ( ! RedisConnect() )
		return false;

	redisReply *reply;

	/* Check the DB is emtpy */
	reply = reinterpret_cast<redisReply*>(redisCommand(rcontext,"DBSIZE"));
	if (reply == NULL)
		goto redis_error;
	if (reply->type != REDIS_REPLY_INTEGER || reply->integer != 0)
		MsgThread::Warning("Database is not empty\n");
	freeReplyObject(reply);

	/* PING server */
	reply = reinterpret_cast<redisReply*>(redisCommand(rcontext,"PING"));
	if (reply == NULL)
		goto redis_error;
	MsgThread::Info("PING");
	MsgThread::Info(reply->str);
	freeReplyObject(reply);

	return true;

redis_error:
	Error(rcontext->errstr);
	return false;
	}

bool Redis::DoFinish(double network_time)
{
	SendRedisBatch();
	if ( rcontext != NULL )
		redisFree(rcontext);
	return true;
}

uint64 Redis::SendRedisBatch(uint nretries)
	{
	if ( rcontext == NULL )
		return enqueued_cntr;

	redisReply *reply;

	while ( enqueued_cntr > 0 && nretries > 0 )
		{
		reply = NULL;
		nretries--;

		while ( rcontext->err && nretries > 0 )
			{
			MsgThread::Warning("Reconnecting to REDIS DB, losing data!!!");
			redisFree(rcontext);
			if ( ! RedisConnect() )
				{
				nretries--;
				continue;
				}
			// Reconnect losses enqueued cmds!
			enqueued_cntr = 0;
			MsgThread::Info("Reconnected!");
			break;
			}

		while ( enqueued_cntr > 0 && nretries > 0 )
			{
			if ( redisGetReply(rcontext, reinterpret_cast<void**>(&reply)) != REDIS_OK || reply == NULL )
				break;
			if ( reply->type == REDIS_REPLY_ERROR )
				{
				Error(Fmt("Redis reply error: %s", reply->str));
				freeReplyObject(reply);
				break;
				}
			freeReplyObject(reply);
			enqueued_cntr--;
			}
		}
	return enqueued_cntr;
	}

bool Redis::DoFlush(double network_time)
	{
	if ( SendRedisBatch() > 0 )
		return false;
	return true;
	}

bool Redis::DoWrite(int num_fields, const threading::Field* const* fields,
			threading::Value** vals)
	{
	key_desc.Clear();
	key_desc.AddRaw(key_prefix);
	if ( ! key_formatter->Describe(&key_desc, vals[key_index]) )
		return false;

	val_desc.Clear();
	if ( ! val_formatter->Describe(&val_desc, num_fields, fields, vals) )
		return false;

	const char* key_bytes = (const char*)key_desc.Bytes();
	const char* val_bytes = (const char*)val_desc.Bytes();
	if ( key_expire == 0 )
		redisAppendCommand(rcontext, "SET %s %s", key_bytes, val_bytes);
	else
		redisAppendCommand(rcontext, "SET %s %s EX %d", key_bytes, val_bytes, key_expire);
	enqueued_cntr++;

	if ( ! buffered )
		if ( SendRedisBatch() > 0 )
			return false;

	return true;
	}

bool Redis::DoRotate(const char* rotated_path, double open, double close, bool terminating)
	{
	FinishedRotation();
	return true;
	}

bool Redis::DoSetBuf(bool enabled)
	{
	buffered = enabled;
	return true;
	}

bool Redis::DoHeartbeat(double network_time, double current_time)
	{
	if ( last_flush_ts + flush_period > current_time )
		return true;

	if ( SendRedisBatch() > 0 )
		return false;

	last_flush_ts = current_time;

	return true;
	}
