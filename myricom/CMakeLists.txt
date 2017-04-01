cmake_minimum_required(VERSION 2.6.3)

project(bro-plugins-myricom)

include(BroPlugin)

find_package(Myricom)

set(VERSION "2.5.0")

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

# If this CMake project is a sub-project of another, we will not
# configure the generic packaging because CPack will fail in the case
# that the parent project has already configured packaging
include(PackageMyricom)
PackageMyricom(${VERSION} )
