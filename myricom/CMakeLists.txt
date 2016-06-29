cmake_minimum_required(VERSION 2.6.3)

project(Plugin)

include(BroPlugin)

find_package(Myricom)

if ( MYRICOM_FOUND )
    include_directories(BEFORE ${MYRICOM_INCLUDE_DIR})
    bro_plugin_begin(Bro Myricom)
    bro_plugin_cc(src/Plugin.cc)
    bro_plugin_cc(src/Myricom.cc)
    bro_plugin_bif(src/myricom.bif)
    bro_plugin_link_library(${MYRICOM_LIBRARY})
    bro_plugin_dist_files(broctl/myricom.py)
    bro_plugin_end()
    message(STATUS "Myricom prefix      : ${MYRICOM_ROOT_DIR}")
else ()
    message(FATAL_ERROR "Myricom library and/or headers not found.")
endif ()
