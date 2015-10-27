@load base/protocols/conn
@load ./tcp_functions

module TCPRECOVERY;

export {
    redef enum Log::ID += { LOG };

    # TCP Recovery record
    type Info: record {
        ts:     time    &log;
        uid:    string  &log;
        id:     conn_id &log;

        label:  string  &log;
        seq:    count   &log;
        rtt:    double  &log;
        state:  int     &log;
        orig:   bool    &log;
        nnseq:  count   &log;
    };

    # TCP Recovery record event
    global log_tcp_recovery: event(rec: Info);
}


# TCP Recovery log init
event bro_init() &priority=5
    {
        Log::create_stream(TCPRECOVERY::LOG, [$columns=Info]);
    }

# TCP limited transmit event
#
# This is generated when TCPRS detects the presence of a limited transmit 
#   event in a connection
#
# c:         connection
# timestamp: network time when the event occurred
# seq:       TCP sequence number associated with the event
# is_orig:   Is this from the originating endpoint?
# rtt:       round-trip time of the connection
# state:     congestion state of the connection
# o_seq:     non-normalized TCP sequence number
# beg_seq:   reserved
# end_seq:   reserved
event TCPRS::conn_limited_transmit(
    c:         connection,
    timestamp: time,
    seq:       count,
    is_orig:   bool,
    rtt:       double,
    state:     int,
    o_seq:     count,
    beg_seq:   count,
    end_seq:   count)
    {
        local rec: TCPRECOVERY::Info = [
            $ts    = timestamp,
            $uid   = c$uid,
            $id    = c$id,
            $label = "TCP::LIMITEDTRANSMIT",
            $seq   = seq,
            $rtt   = rtt,
            $state = state,
            $orig  = is_orig,
            $nnseq = o_seq];
        Log::write(TCPRECOVERY::LOG, rec);
    }

    
# TCP fast recovery event
# 
# This is generated when TCPRS detects the presence of a fast recovery
#   event in a connection
#   
# c:         connection
# timestamp: network time when the event occurred
# seq:       TCP sequence number associated with the event
# is_orig:   Is this from the originating endpoint?
# rtt:       round-trip time of the connection
# state:     congestion state of the connection
# o_seq:     non-normalized TCP sequence number
# beg_seq:   reserved
# end_seq:   reserved
event TCPRS::conn_fast_recovery(
    c:         connection,
    timestamp: time,
    seq:       count,
    is_orig:   bool,
    rtt:       double,
    state:     int,
    o_seq:     count,
    beg_seq:   count,
    end_seq:   count)
    {
        local rec: TCPRECOVERY::Info = [
            $ts    = timestamp,
            $uid   = c$uid,
            $id    = c$id,
            $label = "TCP::FASTRECOVERY",
            $seq   = seq,
            $rtt   = rtt,
            $state = state,
            $orig  = is_orig,
            $nnseq = o_seq];
        Log::write(TCPRECOVERY::LOG, rec);
    }
