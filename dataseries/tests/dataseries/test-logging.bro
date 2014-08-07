#
# @TEST-REQUIRES: which ds2txt
#
# @TEST-EXEC: bro -b Bro::DataSeries %INPUT Log::default_writer=Log::WRITER_DATASERIES 
# @TEST-EXEC: ds2txt --skip-index test.ds >test.ds.txt
# @TEST-EXEC: btest-diff test.ds.txt

module Test;

export {
	redef enum Log::ID += { LOG };

	type Log: record {
		t: time;
		id: conn_id; # Will be rolled out into individual columns.
		status: string &optional;
		country: string &default="unknown";
	} &log;
}

event bro_init()
{
	Log::create_stream(Test::LOG, [$columns=Log]);

        local cid = [$orig_h=1.2.3.4, $orig_p=1234/tcp, $resp_h=2.3.4.5, $resp_p=80/tcp];

	Log::write(Test::LOG, [$t=network_time(), $id=cid, $status="success"]);
	Log::write(Test::LOG, [$t=network_time(), $id=cid, $status="failure", $country="US"]);
	Log::write(Test::LOG, [$t=network_time(), $id=cid, $status="failure", $country="UK"]);
	Log::write(Test::LOG, [$t=network_time(), $id=cid, $status="success", $country="BR"]);
	Log::write(Test::LOG, [$t=network_time(), $id=cid, $status="failure", $country="MX"]);
	
}

