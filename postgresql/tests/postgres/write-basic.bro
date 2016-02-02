# @TEST-EXEC: initdb postgres
# @TEST-EXEC: perl -pi.bak -E "s/#port =.*/port = 7772/;" postgres/postgresql.conf
# @TEST-EXEC: pg_ctl start -D postgres -l serverlog
# @TEST-EXEC: createdb -p 7772 testdb
# @TEST-EXEC: bro %INPUT || true
# @TEST-EXEC: echo "select * from ssh" | psql -A -p 7772 testdb >ssh.out 2>&1 || true
# TEST-EXEC: pg_dump -p 7772 -a testdb > ssh.out 2>&1 || true
# @TEST-EXEC: pg_ctl stop -D postgres -m fast
# @TEST-EXEC: btest-diff ssh.out

# Test all possible types.

module SSHTest;

export {
	redef enum Log::ID += { LOG };

	type Log: record {
		b: bool;
		i: int;
		e: Log::ID;
		c: count;
		p: port;
		sn: subnet;
		a: addr;
		d: double;
		t: time;
		iv: interval;
		s: string;
		sc: set[count];
		ss: set[string];
		se: set[string];
		vc: vector of count;
		ve: vector of string;
		f: function(i: count) : string;
	} &log;
}

function foo(i : count) : string
	{
	if ( i > 0 )
		return "Foo";
	else
		return "Bar";
	}

event bro_init()
{
	Log::create_stream(SSHTest::LOG, [$columns=Log]);
	local filter: Log::Filter = [$name="postgres", $path="ssh", $writer=Log::WRITER_POSTGRESQL, $config=table(["dbname"]="testdb", ["port"]="7772")];
	Log::add_filter(SSHTest::LOG, filter);

	local empty_set: set[string];
	local empty_vector: vector of string;

	Log::write(SSHTest::LOG, [
		$b=T,
		$i=-42,
		$e=SSH::LOG,
		$c=21,
		$p=123/tcp,
		$sn=10.0.0.1/24,
		$a=1.2.3.4,
		$d=3.14,
		$t=network_time(),
		$iv=100secs,
		$s="hurz",
		$sc=set(1,2,3,4),
		$ss=set("AA", "BB", "CC"),
		$se=empty_set,
		$vc=vector(10, 20, 30),
		$ve=empty_vector,
		$f=foo
		]);
}

