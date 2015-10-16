# - Try to find Myricom includes.
#
# Variables used by this module, they can change the default behaviour and need
# to be set before calling find_package:
#
#  Myricom_ROOT_DIR          Set this variable to the root installation of
#                            pf_ring if the module has problems finding the
#                            proper installation path.
#
# Variables defined by this module:
#
#  Myricom_FOUND               System has pf_ring API files.
#  Myricom_INCLUDE_DIR         The pf_ring include directory.
#  Myricom_LIBRARY             The libpfring library

find_path(MYRICOM_ROOT_DIR
    NAMES include/snf.h
)

find_path(MYRICOM_INCLUDE_DIR
    NAMES snf.h
    HINTS ${MYRICOM_ROOT_DIR}/include
)

find_library(MYRICOM_LIBRARY
    NAMES snf
    HINTS ${MYRICOM_ROOT_DIR}/lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MYRICOM DEFAULT_MSG
    MYRICOM_LIBRARY
    MYRICOM_INCLUDE_DIR
)

mark_as_advanced(
    MYRICOM_ROOT_DIR
    MYRICOM_INCLUDE_DIR
    MYRICOM_LIBRARY
)

