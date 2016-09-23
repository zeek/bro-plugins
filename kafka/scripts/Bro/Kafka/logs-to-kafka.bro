##! load this script to enable log output to kafka

module Kafka;

export {
	##
	## which log streams should be sent to kafka?
	## example:
	##		redef Kafka::logs_to_send = set(Conn::Log, HTTP::LOG, DNS::LOG);
	##
	const logs_to_send: set[Log::ID] &redef;
}

event bro_init() &priority=-5
{
	for (stream_id in Log::active_streams)
	{
		if (stream_id in Kafka::logs_to_send)
		{
			local filter: Log::Filter = [
				$name = fmt("kafka-%s", stream_id),
				$writer = Log::WRITER_KAFKAWRITER,
				$config = table(["stream_id"] = fmt("%s", stream_id))
			];

			Log::add_filter(stream_id, filter);
		}
	}
}
