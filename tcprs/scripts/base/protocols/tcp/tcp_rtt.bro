@load base/protocols/conn

module TCPRTT;

export {
    redef enum Log::ID += { LOG };

    # TCP round-trip time record
    type Info: record {
        ts:         time    &log;
        uid:        string  &log;
        id:         conn_id &log;

        label:      string  &log;
        rtt:        double  &log &optional;
        rto:        double  &log &optional;
        is_orig:    bool    &log;
    };

    # TCP round-trip time record event
    global log_tcp_rtt: event(rec: Info);
}

# TCP round-trip time log init
event bro_init() &priority=5
    {
        Log::create_stream(TCPRTT::LOG, [$columns=Info]);
    }

# TCP initial retransmission timeout measurement event
#
# This event is generated on detection of the first retransmission timeout
#   of a connection
#
# c:         connection
# timestamp: network time when the event was observed
# rto:       initial RTO measurement
# is_orig:   Was the RTO observed from the originating endpoint?
event conn_initial_rto(
    c:         connection,
    timestamp: time,
    rto:       double,
    is_orig:   bool) &priority=-5
    {
        local rec: TCPRTT::Info = [
            $ts      = timestamp,
            $uid     = c$uid,
            $id      = c$id,
            $label   = "TCP::InitialRTO",
            $rto     = rto,
            $is_orig = is_orig ];
        Log::write(TCPRTT::LOG, rec);
    }

# TCP initial round-trip time measurement event
#
# This event is generated on detection of the first round-trip time sample
#   of a connection
#
# c:         connection
# timestamp: network time when the event was observed
# rtt:       initial RTO measurement
# is_orig:   Was the RTO observed from the originating endpoint?
event TCPRS::conn_initial_rtt(
    c: connection,
    timestamp: time,
    rtt:double,
    is_orig:bool) &priority=-5
    {
        local rec: TCPRTT::Info = [
            $ts      = timestamp,
            $uid     = c$uid,
            $id      = c$id,
            $label   = "TCP::InitialRTT",
            $rtt     = rtt,
            $is_orig = is_orig ];
        Log::write(TCPRTT::LOG, rec);
    }
