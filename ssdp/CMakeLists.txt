project(Bro-Plugin-STUN)

cmake_minimum_required(VERSION 2.8)

if ( NOT BRO_DIST )
    message(FATAL_ERROR "BRO_DIST not set")
endif ()

set(CMAKE_MODULE_PATH ${BRO_DIST}/cmake)

include(BroPlugin)

#include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

bro_plugin_begin(Bro SSDP)
bro_plugin_cc(src/Plugin.cc)
bro_plugin_cc(src/SSDP.cc)
bro_plugin_bif(src/events.bif)
bro_plugin_pac(src/ssdp.pac src/ssdp-analyzer.pac src/ssdp-protocol.pac)
bro_plugin_end()
