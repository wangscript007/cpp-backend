find_path(UV_INCLUDE_DIRS NAMES uv.h)
find_library(UV_LIBRARIES NAMES uv libuv)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(UV DEFAULT_MSG UV_LIBRARIES UV_INCLUDE_DIRS)

if(UV_INCLUDE_DIRS AND UV_LIBRARIES)
    SET(UV_FOUND TRUE)
else()
    SET(UV_FOUND FALSE)
endif()

if (NOT UV_FOUND)
    message(FATAL_ERROR "Couldn't find libuv.")
endif()

mark_as_advanced(UV_LIBRARIES UV_INCLUDE_DIRS)
