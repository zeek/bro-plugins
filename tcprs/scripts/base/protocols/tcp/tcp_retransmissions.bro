@load base/protocols/conn
@load ./tcp_functions

module TCPRETRANSMISSIONS;

export {
    redef enum Log::ID += { LOG };

    # TCP Retransmissions record
    type Info: record {
        ts:         time    &log;
        uid:        string  &log;
        id:         conn_id &log;

        label:      string  &log;
        seq:        count   &log;
        rtt:        double  &log;
        state:      int     &log;
        orig:       bool    &log;
        reason:     int     &log;
        rtype:      int     &log;
        confidence: double  &log &optional;
        nnseq:      count   &log;
        flags:      int     &log &optional;
    };

    # TCP Retransmissions record event
    global log_tcp_retransmissions: event(rec: Info);
}

# TCP Retransmissions log init
event bro_init() &priority=5
    {
        Log::create_stream(TCPRETRANSMISSIONS::LOG, [$columns=Info]);
    }

# TCP retransmission event
#
# This is generated when TCPRS detects the presence of a retransmission
#   event in a connection
#
# c:          connection
# timestamp:  network time when the event occurred
# seq:        TCP sequence number associated with the event
# is_orig:    Is this from the originating endpoint?
# rtt:        round-trip time of the connection
# state:      congestion state of the connection
# o_seq:      non-normalized TCP sequence number
# beg_seq:    reserved
# end_seq:    reserved
# reason:     reason code for why the retransmission was sent
# rtype:      retransmission type code
# confidence: confidence value associated with the event
# flags:      event flags 
event TCPRS::conn_rexmit(
    c:          connection,
    timestamp:  time,
    seq:        count,
    is_orig:    bool,
    rtt:        double,
    state:      int,
    o_seq:      count,
    beg_seq:    count,
    end_seq:    count,
    reason:     int,
    rtype:      int,
    confidence: double,
    flags:      int) &priority=-5
    {
        local rec: TCPRETRANSMISSIONS::Info = [
            $ts          = timestamp,
            $uid         = c$uid,
            $id          = c$id,
            $label       = "TCP::Retransmissions",
            $seq         = seq,
            $rtt         = rtt,
            $state       = state,
            $orig        = is_orig,
            $reason      = reason,
            #$confidence = confidence,
            $rtype       = rtype,
            $nnseq       = o_seq,
            $flags       = flags];
        Log::write(TCPRETRANSMISSIONS::LOG, rec);
    }

# TCP spurious duplicate acknowledgment event
# 
# This is generated when TCPRS detects a spurious duplicate acknowledgment.
# 
# c:          connection
# timestamp:  network time when the event occurred
# seq:        TCP sequence number associated with the event
# is_orig:    Is this from the originating endpoint?
# rtt:        round-trip time of the connection
# state:      congestion state of the connection
# o_seq:      non-normalized TCP sequence number
# beg_seq:    reserved
# end_seq:    reserved
# reason:     reason code for why the retransmission was sent
# rtype:      retransmission type code
event TCPRS::conn_spurious_dsack(
    c:         connection,
    timestamp: time,
    seq:       count,
    is_orig:   bool,
    rtt:       double,
    state:     int,
    o_seq:     count,
    beg_seq:   count,
    end_seq:   count,
    reason:    int,
    rtype:     int)
    {
            local rec: TCPRETRANSMISSIONS::Info = [
            $ts     = timestamp,
            $uid    = c$uid,
            $id     = c$id,
            $label  = "TCP::SpuriousRetransmission",
            $seq    = seq,
            $rtt    = rtt,
            $state  = state,
            $orig   = is_orig,
            $reason = reason,
            $rtype  = rtype,
            $nnseq  = o_seq];
            Log::write(TCPRETRANSMISSIONS::LOG, rec);
    }
