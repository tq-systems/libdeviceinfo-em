find_path(JANSSON_INCLUDE_DIR jansson.h /usr/include /usr/local/include)

find_library(JANSSON_LIBRARY NAMES jansson PATHS /usr/lib /usr/local/lib)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Jansson DEFAULT_MSG JANSSON_LIBRARY JANSSON_INCLUDE_DIR)

mark_as_advanced(JANSSON_INCLUDE_DIR JANSSON_LIBRARY)
