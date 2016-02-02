# @TEST-EXEC: initdb postgres
# @TEST-EXEC: perl -pi.bak -E "s/#port =.*/port = 7772/;" postgres/postgresql.conf
# @TEST-EXEC: pg_ctl start -D postgres -l serverlog
# @TEST-EXEC: createdb -p 7772 testdb
# @TEST-EXEC: psql -p 7772 testdb < dump.sql || true
# @TEST-EXEC: btest-bg-run bro bro %INPUT
# @TEST-EXEC: btest-bg-wait 10 || true
# @TEST-EXEC: pg_ctl stop -D postgres -m fast
# @TEST-EXEC: btest-diff out

@TEST-START-FILE dump.sql
CREATE TABLE ssh (
    id integer NOT NULL,
    b boolean,
    i integer,
    e text,
    c integer,
    p integer,
    sn inet,
    a inet,
    d double precision,
    t double precision,
    iv double precision,
    s text,
    sc integer[],
    ss text[],
    se text[],
    vc integer[],
    ve text[],
    f text
);

CREATE SEQUENCE ssh_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

ALTER SEQUENCE ssh_id_seq OWNED BY ssh.id;
ALTER TABLE ONLY ssh ALTER COLUMN id SET DEFAULT nextval('ssh_id_seq'::regclass);

COPY ssh (id, b, i, e, c, p, sn, a, d, t, iv, s, sc, ss, se, vc, ve, f) FROM stdin;
1	t	-42	SSH::LOG	21	123	10.0.0.0/24	1.2.3.4	3.14000000000000012	1454444233.58016205	100	hurz	{2,4,1,3}	{'CC','AA','BB'}	\N	{10,20,30}	\N	SSHTest::foo\n{ \nif (0 < SSHTest::i) \n\treturn (Foo);\nelse\n\treturn (Bar);\n\n}
\.

SELECT pg_catalog.setval('ssh_id_seq', 1, true);
ALTER TABLE ONLY ssh
    ADD CONSTRAINT ssh_id_key UNIQUE (id);

@TEST-END-FILE


redef exit_only_after_terminate = T;

global outfile: file;


type InfoType: record {
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
#		f: function(i: count) : string;
};

event line(description: Input::EventDescription, tpe: Input::Event, r: InfoType)
	{
	print outfile, r;
	}

event bro_init()
	{
	outfile = open("../out");
	Input::add_event([$source="select * from ssh;", $name="postgres", $fields=InfoType, $ev=line, $want_record=T,
		$reader=Input::READER_POSTGRESQL, $config=table(["dbname"]="testdb", ["port"]="7772")]);
	}

event Input::end_of_data(name: string, source:string)
	{
	print outfile, "End of data";
	close(outfile);
	terminate();
	}


