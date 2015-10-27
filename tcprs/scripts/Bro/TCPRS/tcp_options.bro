module TCPOPTIONS;

export {
    redef enum Log::ID += { LOG };

    # TCP Options record
    type Info: record {
        ts:                 time    &log;
        uid:                string  &log;
        id:                 conn_id &log;
        label:              string  &log;
        timestamps:         bool    &log;
        bad:                bool    &log;
        sack_used:          bool    &log;
        orig_sack_offer:    bool    &log;
        resp_sack_offer:    bool    &log;
    };

    # TCP options record event
    global log_tcp_options: event(rec: Info);
}

# TCP options log init
event bro_init() &priority=5
    {
        Log::create_stream(TCPOPTIONS::LOG, [$columns=Info]);
    }

# TCP connection configuration event
#
# This event is generated on connection close, displaying the options used 
#   by the connection.
#
# c:            connection
# timestamp:    network time when the event was generated
# ts:           Is the timestamp option enabled?
# bad_conn:     Did this connection exhibit behavior that would make it 
#               unreliable for analysis?
# sack:         Is the selective acknowledgment option in use?
# o_sack_offer: Did the originating endpoint offer sack?
# r_sack_offer: Did the responding endpoint offer sack?
event TCPRS::conn_config(
        c:            connection,
        timestamp:    time,
        ts:           bool,
        bad_conn:     bool,
        sack:         bool,
        o_sack_offer: bool,
        r_sack_offer: bool) &priority=-5
    {
        local rec: TCPOPTIONS::Info = [
            $ts                 = timestamp,
            $uid                = c$uid,
            $id                 = c$id,
            $label              = "TCP::Options",
            $timestamps         = ts,
            $bad                = bad_conn,
            $sack_used          = sack,
            $orig_sack_offer    = o_sack_offer,
            $resp_sack_offer    = r_sack_offer ];
        Log::write(TCPOPTIONS::LOG, rec);
    }
