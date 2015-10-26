@TEST-EXEC: bro -C -r ${TRACES}/tcp/composite/comp024.trace Bro/TCPRS
@TEST-EXEC: btest-diff conn.log
@TEST-EXEC: btest-diff tcpreordering.log
@TEST-EXEC-FAIL: test -f tcpdeadconnection.log
@TEST-EXEC: btest-diff tcprecovery.log
@TEST-EXEC: btest-diff tcpretransmissions.log
@TEST-EXEC: btest-diff tcpoptions.log

