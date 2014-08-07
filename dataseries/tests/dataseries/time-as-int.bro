#
# @TEST-REQUIRES: which ds2txt
# @TEST-GROUP: dataseries
#
# @TEST-EXEC: bro -r ${BRO_TRACES}/wikipedia.trace Bro::DataSeries %INPUT Log::default_writer=Log::WRITER_DATASERIES
# @TEST-EXEC: ds2txt --skip-index conn.ds >conn.ds.txt
# @TEST-EXEC: btest-diff conn.ds.txt

redef LogDataSeries::use_integer_for_time = T;
