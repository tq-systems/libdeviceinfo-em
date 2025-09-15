include(GNUInstallDirs)

set_property(DIRECTORY PROPERTY COMPILE_DEFINITIONS "_POSIX_C_SOURCE=200809L")
include_directories(${LIBDEVICEINFO_SOURCE_DIR}/include ${JANSSON_INCLUDE_DIR})

add_library(deviceinfo
	deviceinfo.c
)
set_target_properties(deviceinfo PROPERTIES
	SOVERSION 0
)
target_link_libraries(deviceinfo ${JANSSON_LIBRARY})

add_executable(test-deviceinfo
	test-deviceinfo.c
)
target_link_libraries(test-deviceinfo deviceinfo)


add_executable(em-devinfo
	em-devinfo.c
)
target_link_libraries(em-devinfo deviceinfo)

install(TARGETS deviceinfo
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(TARGETS em-devinfo
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
