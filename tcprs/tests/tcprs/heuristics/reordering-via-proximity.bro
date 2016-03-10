@TEST-EXEC: bro -C -r ${TRACES}/tcp/heuristics/reordering-via-proximity.trace Bro/TCPRS
@TEST-EXEC-FAIL: test -f tcpreordering.log
@TEST-EXEC: grep "1074220183.128129" tcpretransmissions.log > is_not_empty.log
@TEST-EXEC: test -s is_not_empty.log
@TEST-EXEC: rm is_not_empty.log
@TEST-EXEC: btest-diff tcpretransmissions.log
