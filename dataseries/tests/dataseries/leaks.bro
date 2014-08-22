# Needs perftools support.
#
# @TEST-REQUIRES: which ds2txt
# @TEST-REQUIRES: bro  --help 2>&1 | grep -q mem-leaks
#
# @TEST-GROUP: leaks
#
# @TEST-REQUIRES: bro --help 2>&1 | grep -q mem-leaks
# @TEST-EXEC: HEAP_CHECK_DUMP_DIRECTORY=. HEAPCHECK=local btest-bg-run bro bro -m -r ${BRO_TRACES}/wikipedia.trace Bro::DataSeries Log::default_writer=Log::WRITER_DATASERIES
# @TEST-EXEC: btest-bg-wait 25
