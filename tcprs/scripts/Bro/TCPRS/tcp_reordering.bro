@load base/protocols/conn

module TCPREORDERING;

export {
    redef enum Log::ID += { LOG };

    type Info: record {
        ts:    					time 	&log;
        uid:    				string	&log;
        id:    					conn_id	&log;

        label:    				string  &log;
        seq:    				count   &log;
        gap:    				double 	&log;
        rtt:    				double	&log &optional;
        segments_outoforder:    int		&log;
        orig:    				bool	&log;
        ambiguous:    			bool	&log &optional;
        nnseq:    				count	&log;
    };

    global log_tcp_reordering: event(rec: Info);
}

event bro_init() &priority=5
    {
        Log::create_stream(TCPREORDERING::LOG, [$columns=Info]);
    }

    
# TCP reordering event
# 
# This is generated when TCPRS detects the presence of a reordering
#   event in a connection
#   
# c:         connection
# timestamp: network time when the event occurred
# is_orig:   Is this from the originating endpoint?
# seq:       TCP sequence number associated with the event
# gap:       how long ago was the sequence gap detected?
# rtt:       round-trip time of the connection
# num_seq:   number of segments detected since the sequence gap was
#            detected
# o_seq:     non-normalized TCP sequence number
# beg_seq:   reserved
# end_seq:   reserved
event TCPRS::conn_ooo_event(
    c:         connection,
    timestamp: time,
    is_orig:   bool,
    seq:       count,
    gap:       double,
    rtt:       double,
    num_seq:   int,
    o_seq:     count,
    beg_seq:   count,
    end_seq:   count) &priority=-5
    {
        local rec: TCPREORDERING::Info = [
            $ts           		 = timestamp,
            $uid          		 = c$uid,
            $id           		 = c$id,
            $label        		 = "TCP::Reordering",
            $seq          		 = seq,
            $gap          		 = gap,
            $rtt           		 = rtt,
            $segments_outoforder = num_seq,
            $orig         		 = is_orig,
            $ambiguous			 = F,
            $nnseq         		 = o_seq ];
        Log::write(TCPREORDERING::LOG, rec);
    }

# TCP ambiguous reordering event
#
# This is generated when TCPRS detects the presence of an ambiguous 
#   reordering event in a connection
#
# c:         connection
# timestamp: network time when the event occurred
# is_orig:   Is this from the originating endpoint?
# seq:       TCP sequence number associated with the event
# gap:       how long ago was the sequence gap detected?
# rtt:       round-trip time of the connection
# num_seq:   number of segments detected since the sequence gap was
#            detected
# o_seq:     non-normalized TCP sequence number
# beg_seq:   reserved
# end_seq:   reserved
event TCPRS::conn_ambi_order(
    c:         connection,
    timestamp: time,
    is_orig:   bool,
    seq:       count,
    gap:       double,
    num_seq:   int,
    o_seq:     count,
    beg_seq:   count,
    end_seq:   count) &priority=-5
    {
        local rec: TCPREORDERING::Info = [
            $ts           		 = timestamp,
            $uid              	 = c$uid,
            $id               	 = c$id,
            $label            	 = "TCP::Reordering",
            $seq              	 = seq,
            $gap              	 = gap,
            $segments_outoforder = num_seq,
            $orig             	 = is_orig,
            $ambiguous             = T,
            $nnseq             	 = o_seq ];
        Log::write(TCPREORDERING::LOG, rec);
    }
