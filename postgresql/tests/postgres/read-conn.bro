# @TEST-SERIALIZE: postgres
# @TEST-EXEC: initdb postgres
# @TEST-EXEC: perl -pi.bak -E "s/#port =.*/port = 7772/;" postgres/postgresql.conf
# @TEST-EXEC: pg_ctl start -D postgres -l serverlog
# @TEST-EXEC: sleep 5
# @TEST-EXEC: createdb -p 7772 testdb
# @TEST-EXEC: psql -p 7772 testdb < dump.sql || true
# @TEST-EXEC: btest-bg-run bro bro %INPUT
# @TEST-EXEC: btest-bg-wait 10 || true
# @TEST-EXEC: pg_ctl stop -D postgres -m fast
# @TEST-EXEC: btest-diff out

@TEST-START-FILE dump.sql
CREATE TABLE conn (
    id integer NOT NULL,
    ts double precision,
    uid text,
    "id$orig_h" inet,
    "id$orig_p" integer,
    "id$resp_h" inet,
    "id$resp_p" integer,
    proto text,
    service text,
    duration double precision,
    orig_bytes integer,
    resp_bytes integer,
    conn_state text,
    local_orig boolean,
    local_resp boolean,
    missed_bytes integer,
    history text,
    orig_pkts integer,
    orig_ip_bytes integer,
    resp_pkts integer,
    resp_ip_bytes integer,
    tunnel_parents text[]
);
CREATE SEQUENCE conn_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
ALTER SEQUENCE conn_id_seq OWNED BY conn.id;

COPY conn (id, ts, uid, "id$orig_h", "id$orig_p", "id$resp_h", "id$resp_p", proto, service, duration, orig_bytes, resp_bytes, conn_state, local_orig, local_resp, missed_bytes, history, orig_pkts, orig_ip_bytes, resp_pkts, resp_ip_bytes, tunnel_parents) FROM stdin;
1	1300475167.09653497	CHhAvVGS1DHFjwGM9	141.142.220.202	5353	224.0.0.251	5353	udp	dns	\N	\N	\N	S0	\N	\N	0	D	1	73	0	0	{a,b}
2	1300475167.09701204	ClEkJM2Vm5giqnMf4h	fe80::217:f2ff:fed7:cf65	5353	ff02::fb	5353	udp	dns	\N	\N	\N	S0	\N	\N	0	D	1	199	0	0	\N
3	1300475167.09981608	C4J4Th3PJpwUYZZ6gc	141.142.220.50	5353	224.0.0.251	5353	udp	dns	\N	\N	\N	S0	\N	\N	0	D	1	179	0	0	\N
4	1300475168.853899	CmES5u32sYpV7JYN	141.142.220.118	43927	141.142.2.2	53	udp	dns	0.000435000000000000003	38	89	SF	\N	\N	0	Dd	1	66	1	117	\N
5	1300475168.85437799	CP5puj4I8PtEU4qzYg	141.142.220.118	37676	141.142.2.2	53	udp	dns	0.000420000000000000017	52	99	SF	\N	\N	0	Dd	1	80	1	127	\N
6	1300475168.85483694	C37jN32gN3y3AZzyf6	141.142.220.118	40526	141.142.2.2	53	udp	dns	0.000391999999999999987	38	183	SF	\N	\N	0	Dd	1	66	1	211	\N
7	1300475168.85795593	C0LAHyvtKSQHyJxIl	141.142.220.118	32902	141.142.2.2	53	udp	dns	0.000317000000000000007	38	89	SF	\N	\N	0	Dd	1	66	1	117	\N
8	1300475168.85830593	CFLRIC3zaTU1loLGxh	141.142.220.118	59816	141.142.2.2	53	udp	dns	0.000342999999999999989	52	99	SF	\N	\N	0	Dd	1	80	1	127	\N
9	1300475168.85871291	C9rXSW3KSpTYvPrlI1	141.142.220.118	59714	141.142.2.2	53	udp	dns	0.000375000000000000008	38	183	SF	\N	\N	0	Dd	1	66	1	211	\N
10	1300475168.891644	C9mvWx3ezztgzcexV7	141.142.220.118	58206	141.142.2.2	53	udp	dns	0.000339	38	89	SF	\N	\N	0	Dd	1	66	1	117	\N
11	1300475168.89203691	CNnMIj2QSd84NKf7U3	141.142.220.118	38911	141.142.2.2	53	udp	dns	0.000335000000000000011	52	99	SF	\N	\N	0	Dd	1	80	1	127	\N
12	1300475168.89241409	C7fIlMZDuRiqjpYbb	141.142.220.118	59746	141.142.2.2	53	udp	dns	0.000420999999999999988	38	183	SF	\N	\N	0	Dd	1	66	1	211	\N
13	1300475168.89398789	CpmdRlaUoJLN3uIRa	141.142.220.118	45000	141.142.2.2	53	udp	dns	0.00038400000000000001	38	89	SF	\N	\N	0	Dd	1	66	1	117	\N
14	1300475168.89442205	C1Xkzz2MaGtLrc1Tla	141.142.220.118	48479	141.142.2.2	53	udp	dns	0.000317000000000000007	52	99	SF	\N	\N	0	Dd	1	80	1	127	\N
15	1300475168.89478707	CqlVyW1YwZ15RhTBc4	141.142.220.118	48128	141.142.2.2	53	udp	dns	0.000422999999999999982	38	183	SF	\N	\N	0	Dd	1	66	1	211	\N
16	1300475168.9017489	CBA8792iHmnhPLksKa	141.142.220.118	56056	141.142.2.2	53	udp	dns	0.000402000000000000014	36	131	SF	\N	\N	0	Dd	1	64	1	159	\N
17	1300475168.90219498	CGLPPc35OzDQij1XX8	141.142.220.118	55092	141.142.2.2	53	udp	dns	0.000373999999999999983	36	198	SF	\N	\N	0	Dd	1	64	1	226	\N
18	1300475169.8994379	Cipfzj1BEnhejw8cGf	141.142.220.44	5353	224.0.0.251	5353	udp	dns	\N	\N	\N	S0	\N	\N	0	D	1	85	0	0	\N
19	1300475170.86238408	CV5WJ42jPYbNW9JNWf	141.142.220.226	137	141.142.220.255	137	udp	dns	2.61301700000000015	350	0	S0	\N	\N	0	D	7	546	0	0	\N
20	1300475171.67537189	CPhDKt12KQPUVbQz06	fe80::3074:17d5:2052:c324	65373	ff02::1:3	5355	udp	dns	0.100096000000000004	66	0	S0	\N	\N	0	D	2	162	0	0	\N
21	1300475171.67708111	CAnFrb2Cvxr5T7quOc	141.142.220.226	55131	224.0.0.252	5355	udp	dns	0.100020999999999999	66	0	S0	\N	\N	0	D	2	122	0	0	\N
22	1300475173.11674905	C8rquZ3DjgNW06JGLl	fe80::3074:17d5:2052:c324	54213	ff02::1:3	5355	udp	dns	0.0998010000000000008	66	0	S0	\N	\N	0	D	2	162	0	0	\N
23	1300475173.11736202	CzrZOtXqhwwndQva3	141.142.220.226	55671	224.0.0.252	5355	udp	dns	0.0998489999999999933	66	0	S0	\N	\N	0	D	2	122	0	0	\N
24	1300475173.15367889	CaGCc13FffXe6RkQl9	141.142.220.238	56641	141.142.220.255	137	udp	dns	\N	\N	\N	S0	\N	\N	0	D	1	78	0	0	\N
25	1300475169.7803309	CFSwNi4CNGxcuffo49	141.142.220.235	6705	173.192.163.128	80	tcp	\N	\N	\N	\N	OTH	\N	\N	0	^h	0	0	1	48	\N
26	1300475168.8929131	CykQaM33ztNt0csB9a	141.142.220.118	49999	208.80.152.3	80	tcp	http	0.220960999999999991	1137	733	S1	\N	\N	0	ShADad	6	1457	4	949	\N
27	1300475168.72400689	CUM0KZ3MLUfNB0cl11	141.142.220.118	48649	208.80.152.118	80	tcp	http	0.119904999999999998	525	232	S1	\N	\N	0	ShADad	4	741	3	396	\N
28	1300475168.85532999	CwjjYJ2WqgTbAqiHl6	141.142.220.118	49997	208.80.152.3	80	tcp	http	0.219719999999999999	1125	734	S1	\N	\N	0	ShADad	6	1445	4	950	\N
29	1300475168.85530496	C3eiCBGOLw3VtHfOj	141.142.220.118	49996	208.80.152.3	80	tcp	http	0.218501000000000001	1171	733	S1	\N	\N	0	ShADad	6	1491	4	949	\N
30	1300475168.65200305	CtPZjS20MLrsMUOJi2	141.142.220.118	35634	208.80.152.2	80	tcp	\N	0.0613290000000000016	463	350	OTH	\N	\N	0	DdA	2	567	1	402	\N
31	1300475168.9026351	CiyBAq1bBLNaTiTAc	141.142.220.118	35642	208.80.152.2	80	tcp	http	0.120040999999999995	534	412	S1	\N	\N	0	ShADad	4	750	3	576	\N
32	1300475168.85916305	Ck51lg1bScffFj34Ri	141.142.220.118	49998	208.80.152.3	80	tcp	http	0.215893000000000002	1130	734	S1	\N	\N	0	ShADad	6	1450	4	950	\N
33	1300475168.89293599	CtxTCR2Yer0FR1tIBg	141.142.220.118	50000	208.80.152.3	80	tcp	http	0.229603000000000002	1148	734	S1	\N	\N	0	ShADad	6	1468	4	950	\N
34	1300475168.89526701	CLNN1k2QMum1aexUK7	141.142.220.118	50001	208.80.152.3	80	tcp	http	0.227284000000000014	1178	734	S1	\N	\N	0	ShADad	6	1498	4	950	\N
\.
@TEST-END-FILE


redef exit_only_after_terminate = T;

global outfile: file;


event line(description: Input::EventDescription, tpe: Input::Event, r: Conn::Info)
	{
	print outfile, r;
	}

event bro_init()
	{
	outfile = open("../out");
	Input::add_event([$source="select * from conn;", $name="postgres", $fields=Conn::Info, $ev=line, $want_record=T,
		$reader=Input::READER_POSTGRESQL, $config=table(["dbname"]="testdb", ["port"]="7772")]);
	}

event Input::end_of_data(name: string, source:string)
	{
	print outfile, "End of data";
	close(outfile);
	terminate();
	}


