
cmake_minimum_required(VERSION 2.6.3)

project(Plugin)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

include(BroPlugin)

find_package(Lintel)
find_package(DataSeries)
find_package(LibXML2)

if (LINTEL_FOUND AND DATASERIES_FOUND AND LIBXML2_FOUND)

    include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

    include_directories(BEFORE ${Lintel_INCLUDE_DIR})
    include_directories(BEFORE ${DataSeries_INCLUDE_DIR})
    include_directories(BEFORE ${LibXML2_INCLUDE_DIR})

    bro_plugin_begin(Bro DataSeries)
    bro_plugin_cc(src/DataSeries.cc)
    bro_plugin_cc(src/Plugin.cc)
    bro_plugin_bif(src/dataseries.bif)
    bro_plugin_link_library(${Lintel_LIBRARIES})
    bro_plugin_link_library(${DataSeries_LIBRARIES})
    bro_plugin_link_library(${LibXML2_LIBRARIES})
    bro_plugin_end()
    message(STATUS "DataSeries prefix   : ${DataSeries_ROOT_DIR}")
else ()
    message(FATAL_ERROR "DataSeries not found.")
endif ()
