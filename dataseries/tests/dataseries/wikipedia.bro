#
# @TEST-REQUIRES: which ds2txt
# 
# @TEST-EXEC: bro -r ${BRO_TRACES}/wikipedia.trace Bro::DataSeries Log::default_writer=Log::WRITER_DATASERIES
# @TEST-EXEC: ds2txt --skip-index conn.ds >conn.ds.txt
# @TEST-EXEC: ds2txt --skip-index http.ds >http.ds.txt
# @TEST-EXEC: btest-diff conn.ds.txt
# @TEST-EXEC: btest-diff http.ds.txt
