# Try to find Taglib
# Once done this will define
# TAGLIB_FOUND - System has taglib
# TAGLIB_INCLUDE_DIRS - The taglib include directories
# TAGLIB_LIBRARIES - The libraries needed to use taglib
# TAGLIB_DEFINITIONS - Compiler switches required for using taglib

find_package(PkgConfig)
pkg_check_modules(PC_TAGLIB QUIET taglib)
set(TAGLIB_DEFINITIONS ${PC_TAGLIB_CFLAGS_OTHER})

find_path(TAGLIB_INCLUDE_DIR taglib.h
          HINTS ${PC_TAGLIB_INCLUDEDIR} ${PC_TAGLIB_INCLUDE_DIRS})

find_library(TAGLIB_LIBRARY NAMES tag
             HINTS ${PC_TAGLIB_LIBDIR} ${PC_TAGLIB_LIBRARY_DIRS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(TAGLIB DEFAULT_MSG TAGLIB_LIBRARY TAGLIB_INCLUDE_DIR)

mark_as_advanced(TAGLIB_INCLUDE_DIR TAGLIB_LIBRARY)

set(TAGLIB_LIBRARIES ${TAGLIB_LIBRARY})
set(TAGLIB_INCLUDE_DIRS ${TAGLIB_INCLUDE_DIR})

