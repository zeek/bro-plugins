
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

cmake_minimum_required(VERSION 2.6.3)

project(Plugin)

include(BroPlugin)

find_package(LibCURL)

if (NOT DISABLE_ELASTICSEARCH AND LIBCURL_FOUND)
  bro_plugin_begin(Bro ElasticSearch)
  bro_plugin_cc(src/ElasticSearch.cc)
  bro_plugin_cc(src/Plugin.cc)
  bro_plugin_bif(src/elasticsearch.bif)
  bro_plugin_link_library(${LibCURL_LIBRARIES})
  bro_plugin_end()
endif()
