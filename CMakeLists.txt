cmake_minimum_required(VERSION 3.8.0)
project(LIBDEVICEINFO C)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(jansson REQUIRED)

set(CMAKE_C_FLAGS "-std=c99 -Wall -Wextra ${CMAKE_C_FLAGS}")

option(BUILD_SHARED_LIBS "Build shared library" ON)

add_subdirectory(include)
add_subdirectory(src)
