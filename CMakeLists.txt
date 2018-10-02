cmake_minimum_required(VERSION 3.8.0)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

project(LIBDEVICEINFO C)

option(BUILD_SHARED_LIBS "Build shared library" ON)

add_subdirectory(include)
add_subdirectory(src)
