
find_path(HIREDIS_INCLUDE_DIR
    NAMES
        hiredis.h
    PATHS
        ${CMAKE_SOURCE_DIR}/lib/hiredis/build/include/hiredis
        /usr/local/include
        /usr/local/include/hiredis
        /usr/include
        /usr/include/hiredis
)
mark_as_advanced(HIREDIS_INCLUDE_DIR)

find_library(HIREDIS_LIBRARY
    NAMES
        hiredis
    PATHS
        ${CMAKE_SOURCE_DIR}/lib/hiredis/build/lib
        /usr/local/lib
        /usr/lib
)
mark_as_advanced(HIREDIS_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(HIREDIS DEFAULT_MSG
    HIREDIS_LIBRARY
    HIREDIS_INCLUDE_DIR
)
