
cmake_minimum_required(VERSION 2.8)

project(Plugin)

include(BroPlugin)

bro_plugin_begin(jswaro tcprs)
bro_plugin_cc(src/Plugin.cc src/TCPRS.cc src/TCPRS_Endpoint.cc src/TCPRS_Support.cc src/TCPRS_Debug.cc)
bro_plugin_bif(src/tcprs.bif src/tcprs_const.bif)
bro_plugin_dist_files(README CHANGES COPYING VERSION)
bro_plugin_end()
