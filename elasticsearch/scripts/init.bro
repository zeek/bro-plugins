##! Log writer for sending logs to an ElasticSearch server.
##!
##! Note: This module is in testing and is not yet considered stable!
##!
##! There is one known memory issue.  If your elasticsearch server is
##! running slowly and taking too long to return from bulk insert
##! requests, the message queue to the writer thread will continue
##! growing larger and larger giving the appearance of a memory leak.

module LogElasticSearch;

export {
	## Destination for the ES logs.  Valid options are
	## "direct" to directly connect to ES and "nsq" to
	## transfer the logs into an nsqd instance.
	const destination = "direct" &redef;

	## Name of the ES cluster.
	const cluster_name = "elasticsearch" &redef;

	## ES server.
	const server_host = "127.0.0.1" &redef;

	## ES port.
	const server_port = 9200 &redef;

	## Name of the ES index.
	const index_prefix = "bro" &redef;

	## Should the index names be in UTC or in local time?
	## Setting this to true would be more compatible with Kibana and other tools.
	const index_name_in_utc = F &redef;

	## Format for the index names.
	## Setting this to "%Y.%m.%d-%H" would be more compatible Kibana and other tools.
	const index_name_fmt = "%Y%m%d%H%M" &redef;

	## The ES type prefix comes before the name of the related log.
	## e.g. prefix = "bro\_" would create types of bro_dns, bro_software, etc.
	const type_prefix = "" &redef;

	## The time before an ElasticSearch transfer will timeout. Note that
	## the fractional part of the timeout will be ignored. In particular,
	## time specifications less than a second result in a timeout value of
	## 0, which means "no timeout."
	const transfer_timeout = 2secs;

	## The batch size is the number of messages that will be queued up before
	## they are sent to be bulk indexed.
	const max_batch_size = 1000 &redef;

	## The maximum amount of wall-clock time that is allowed to pass without
	## finishing a bulk log send.  This represents the maximum delay you
	## would like to have with your logs before they are sent to ElasticSearch.
	const max_batch_interval = 1min &redef;

	## The maximum byte size for a buffered JSON string to send to the bulk
	## insert API.
	const max_byte_size = 1024 * 1024 &redef;

	## If the "nsq" destination is given, this is the topic
	## that Bro will push logs into.
	const nsq_topic = "bro_logs" &redef;
}
