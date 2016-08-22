
cmake_minimum_required(VERSION 2.8)

project(Plugin)

include(BroPlugin)

find_package(PostgreSQL)

if (PostgreSQL_FOUND)
    include_directories(BEFORE ${PostgreSQL_INCLUDE_DIRS})
    link_directories(${PostgreSQL_LIBRARY_DIRS})


    bro_plugin_begin(Bro PostgreSQL)
    bro_plugin_cc(src/PostgresWriter.cc)
    bro_plugin_cc(src/PostgresReader.cc)
    bro_plugin_cc(src/Plugin.cc)
    bro_plugin_bif(src/postgresql.bif)
    bro_plugin_dist_files(README COPYING VERSION)
    bro_plugin_link_library(${PostgreSQL_LIBRARIES})
    bro_plugin_end()
    message(STATUS "PostgreSQL includes : ${PostgreSQL_INCLUDE_DIRS}")
else()
    message(FATAL_ERROR "PostgreSQL not found.")
endif()
