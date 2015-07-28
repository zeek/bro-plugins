
cmake_minimum_required(VERSION 2.6.3)

project(Plugin)

include(BroPlugin)

find_package(PF_RING)

if ( PF_RING_FOUND )
    include_directories(BEFORE ${PF_RING_INCLUDE_DIR} ${PF_RING_KERNEL_INCLUDE_DIR})

    bro_plugin_begin(Bro PF_RING)
    bro_plugin_cc(src/Plugin.cc)
    bro_plugin_cc(src/PF_RING.cc)
    bro_plugin_link_library(${PF_RING_LIBRARY})
    bro_plugin_end()
    message(STATUS "PF_RING prefix      : ${PF_RING_ROOT_DIR}")
else ()
    message(FATAL_ERROR "PF_RING not found.")
endif ()
