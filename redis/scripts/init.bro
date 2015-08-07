##! Log writer for sending logs to a Redis server.
##! 
##! Note: This module is in testing and is not yet considered stable! 

module Redis;

export {
	## Redis server.
	const server_host = "127.0.0.1" &redef;

	## Redis server.
	const unix_path = "" &redef;

	## Redis port.
	const server_port = 6379 &redef;

	## Redis DB.
	const db = 3 &redef;

	## Namespace key prefix.
	const key_prefix = "Bro::" &redef;

	## Log field index which will be used as the key.
	const key_index = 0 &redef;

	## JSON time.
	const json_timestamps: JSON::TimestampFormat = JSON::TS_EPOCH &redef;

	## Log to DB flushes (if buffered) in seconds.
	const flush_period: double = 10 &redef;

	## Optional key expiration in seconds.
	const key_expire = 0 &redef;
}
