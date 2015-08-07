cmake_minimum_required(VERSION 2.6.3)
project(Plugin)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

include(BroPlugin)

find_package(Hiredis REQUIRED)

MESSAGE( STATUS "HIREDIS_INCLUDE_DIR:         " ${HIREDIS_INCLUDE_DIR} )
MESSAGE( STATUS "HIREDIS_LIBRARY:         " ${HIREDIS_LIBRARY} )

include_directories(BEFORE ${HIREDIS_INCLUDE_DIR})
bro_plugin_begin(Bro Redis)
bro_plugin_cc(src/Redis.cc)
bro_plugin_cc(src/Plugin.cc)
bro_plugin_bif(src/redis.bif)
bro_plugin_link_library(${HIREDIS_LIBRARY})
bro_plugin_end()
