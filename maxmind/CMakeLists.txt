
cmake_minimum_required(VERSION 2.8)

project(Plugin)

include(BroPlugin)

include(cmake/FindMaxMindDB.cmake)

if ( LIBMAXMINDDB_FOUND )
    include_directories(BEFORE ${LIBMAXMINDDB_INCLUDE_DIR})
    bro_plugin_begin(Bro MaxMind)
    bro_plugin_cc(src/Plugin.cc src/OpaqueMaxMind.cc)
    bro_plugin_bif(src/maxmind.bif)
    bro_plugin_link_library(${LIBMAXMINDDB_LIBRARY})
    bro_plugin_dist_files(README CHANGES COPYING VERSION)
    bro_plugin_end()
else ()
    message(FATAL_ERROR "MaxMindDB not found.")
endif ()