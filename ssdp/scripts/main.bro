module SSDP;

export {
	redef enum Log::ID += { LOG };

	type Info: record {
		## Timestamp for when the event happened.
		ts:     time    &log;
		## Unique ID for the connection.
		uid:    string  &log;
		## The connection's 4-tuple of endpoint addresses/ports.
		id:     conn_id &log;
		## Type of SSDP method (request or response).
		method:		string	&log &optional;
		## Search target (typically, a device or service type).
		search_target:	string	&log &optional;
    		## Device data.
    		## This value should contain a comma-separated list containing
    		## the OS name, OS version, the string "UPnP/1.0," product name,
   	 	## and product version. This is specified by the UPnP vendor.
    		server:                     string  &log &optional;
    		## Advertisement UUID of device.
    		usn:                        string  &log &optional;
    		## URL for UPnP description of device.
    		location:                   string  &log &optional;
    		## Vector of all header fields.
    		headers:            set[string] &log &optional;
	};

	## Event that can be handled to access the SSDP record
	## as it is sent on to the logging framework.
	global log_ssdp: event(rec: Info);
}

redef record connection += {
  	ssdp: Info &optional;
};

global ssdp_uid_cache: set[string];
const ports = { 1900/udp };

event bro_init() &priority=5
	{
	Log::create_stream(SSDP::LOG, [$columns=Info, $ev=log_ssdp]);
	Analyzer::register_for_ports(Analyzer::ANALYZER_SSDP, ports);
	}

function set_session(c: connection, force: bool)
  	{
  	if ( ! c?$ssdp && force == F )
    		c$ssdp = [$ts=network_time(),$id=c$id,$uid=c$uid];
	if ( c?$ssdp && force == T )
		c$ssdp = [$ts=network_time(),$id=c$id,$uid=c$uid];
  	}

function write_log(c: connection)
	{
	local info = c$ssdp;
	Log::write(SSDP::LOG, info);
	}

event ssdp_method(c: connection, method: string) &priority=5
	{
	set_session(c,F);	

	if ( c$uid in ssdp_uid_cache )
		{
		write_log(c);
		set_session(c,T);
		}

	if ( c$uid !in ssdp_uid_cache )
		add ssdp_uid_cache[c$uid];

	c$ssdp$method = method;
	}

event ssdp_header(c: connection, name: string, value: string) &priority=5
	{
	if ( ! c$ssdp?$headers )
		c$ssdp$headers = set();

	if ( name !in c$ssdp$headers )
		add c$ssdp$headers[name];

	if ( name == /[Ss][Tt]/ )
		c$ssdp$search_target = value;
	else if ( name == /([Ss]|[Nn])[Tt]/ )
                c$ssdp$search_target = value;   
	else if ( name == /[Ss][Ee][Rr][Vv][Ee][Rr]/ )
		c$ssdp$server = value;
	else if ( name  == /[Uu][Ss][Nn]/ )
		c$ssdp$usn = value;
	else if ( name == /[Ll][Oo][Cc][Aa][Tt][Ii][Oo][Nn]/ )
		c$ssdp$location = value;
	}

event connection_state_remove(c: connection) &priority=-5
	{
	if ( c?$ssdp && c$uid in ssdp_uid_cache )
		{
		write_log(c);
		delete ssdp_uid_cache[c$uid];
		}
	}
