# @TEST-SERIALIZE: postgres
# @TEST-EXEC: initdb postgres
# @TEST-EXEC: perl -pi.bak -E "s/#port =.*/port = 7772/;" postgres/postgresql.conf
# @TEST-EXEC: pg_ctl start -D postgres -l serverlog
# @TEST-EXEC: sleep 5
# @TEST-EXEC: createdb -p 7772 testdb
# @TEST-EXEC: echo $BROTRACES > brotraces
# @TEST-EXEC: bro -r $BROTRACES/wikipedia.trace Log::default_writer=Log::WRITER_POSTGRESQL %INPUT
# @TEST-EXEC: echo "select * from conn" | psql -A -p 7772 testdb >conn.out 2>&1 || true
# @TEST-EXEC: echo "select * from http" | psql -A -p 7772 testdb >http.out 2>&1 || true
# @TEST-EXEC: pg_ctl stop -D postgres -m fast
# @TEST-EXEC: btest-diff conn.out
# @TEST-EXEC: btest-diff http.out

redef LogPostgres::default_port=7772;
redef LogPostgres::default_dbname="testdb";
