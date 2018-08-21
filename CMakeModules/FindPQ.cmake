find_path(PQ_INCLUDE_DIR NAMES libpq-fe.h)
find_library(PQ_LIBRARIES NAMES libpq.dylib)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PQ DEFAULT_MSG PQ_LIBRARIES PQ_INCLUDE_DIR)

if (PQ_FOUND)
    message(STATUS "Found libpq: ${PQ_LIBRARIES}, ${PQ_INCLUDE_DIR}")
else()
    message(FATAL_ERROR "Couldn't find libpq.")
endif()

mark_as_advanced(PQ_INCLUDE_DIR PQ_LIBRARIES)