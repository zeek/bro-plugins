@load base/protocols/conn
@load ./tcp_functions

module TCPDEADCONNECTION;

export {
    redef enum Log::ID += { LOG };

    # Dead connection record
    type Info: record {
        ts:         time    &log;
        uid:        string  &log;
        id:         conn_id &log;
        label:      string  &log;
        duration:   double  &log;
        state:      int     &log;
        orig:       bool    &log;
    };


    # Definition of dead connection log event
    global log_tcp_deadconnection: event(rec: Info);
}


# Dead connection init
event bro_init() &priority=5
    {
        Log::create_stream(TCPDEADCONNECTION::LOG, [$columns=Info]);
    }

# Dead connection event
#
# c:         connection object
# timestamp: network time of the event
# duration:  how long was the connection dead for?
# state:     what state was the connection in when it died?
# is_orig:   Is this from the originating TCP endpoint
event TCPRS::conn_dead_event(
    c:         connection,
    timestamp: time,
    duration:  double,
    state:     int,
    is_orig:   bool) &priority=-5
    {
        local rec: TCPDEADCONNECTION::Info = [
            $ts         = timestamp,
            $uid        = c$uid,
            $id         = c$id,
            $label      = "TCP::ConnectionFailure",
            $duration   = duration,
            $state      = state,
            $orig       = is_orig ];
        Log::write(TCPDEADCONNECTION::LOG, rec);
    }
