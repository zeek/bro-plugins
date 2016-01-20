# - Try to find the PostgreSQL installation.
#
# Usage of this module as follows:
#
#     find_package(Postgres)
#
# Variables used by this module, they can change the default behaviour and need
# to be set before calling find_package:
#
#  Postgres_ROOT_DIR         Set this variable to the root installation of
#                            postgres if the module has problems finding the
#                            proper installation path.
#
# Variables defined by this module:
#
#  POSTGRES_FOUND              System has PostgreSQL
#  Postgres_LIBRARY            The PostgreSQL library
#  Postgres_INCLUDE_DIRS       Location of header files libpq-fe.h and pg_type.h

find_path(Postgres_ROOT_DIR
    NAMES include/libpq-fe.h
)

find_path(Postgres_INCLUDE_DIR
    NAMES libpq-fe.h
    HINTS ${Postgres_ROOT_DIR}
    PATH_SUFFIXES
        include
        pgsql
        postgresql
)

find_path(Postgres_type_INCLUDE_DIR
    NAMES catalog/pg_type.h
    HINTS ${Postgres_INCLUDE_DIR}
    PATH_SUFFIXES
        pgsql/server
        postgresql/server
        server
        include/server
)

find_library(Postgres_LIBRARY
    NAMES pq
    HINTS ${Postgres_ROOT_DIR}/lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Postgres DEFAULT_MSG
    Postgres_LIBRARY
    Postgres_INCLUDE_DIR
    Postgres_type_INCLUDE_DIR
)

if (POSTGRES_FOUND)
    set(Postgres_INCLUDE_DIRS
        ${Postgres_INCLUDE_DIR} ${Postgres_type_INCLUDE_DIR})
endif ()

mark_as_advanced(
    Postgres_ROOT_DIR
    Postgres_LIBRARY
    Postgres_INCLUDE_DIR
    Postgres_type_INCLUDE_DIR
)
