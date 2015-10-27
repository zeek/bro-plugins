@load base/init-bare.bro

module TCPOSFP;

export {
    redef enum Log::ID += { LOG };

    # TCP OS fingerprint record
    type Info: record {
        uid:        string  &log;
        id:         conn_id &log;
        host:       addr    &log;
        genre:      string  &log &optional;
        detail:     string  &log &optional;
        dist:       count   &log &optional;
        OS:         OS_version &log;
    };


    # TCP OS fingerprint record event
    global log_tcp_osfp: event(rec: Info);
}

# TCP OS fingerprint log init
event bro_init() &priority=5
    {
        Log::create_stream(TCPOSFP::LOG, [$columns=Info]);
    }


# TCP OS fingerprint event
#
# c:    connection
# host: host address
# OS:   operating system version
event OS_version_found(
    c:    connection,
    host: addr,
    OS:   OS_version) &priority=-5
    {
        local rec: TCPOSFP::Info = [
            $uid    = c$uid,
            $id     = c$id,
            $host   = host,
            $genre  = OS$genre,
            $detail = OS$detail,
            $dist   = OS$dist,
            $OS     = OS ];
        Log::write(TCPOSFP::LOG, rec);
    }
