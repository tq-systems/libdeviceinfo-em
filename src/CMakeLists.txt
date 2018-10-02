include_directories(${LIBDEVICEINFO_SOURCE_DIR}/include)

add_library(deviceinfo
	deviceinfo.c
)
set_target_properties(deviceinfo PROPERTIES
  COMPILE_FLAGS "-Wall -Wextra"
)

install(TARGETS deviceinfo
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)
