cmake_minimum_required( VERSION 2.6 )

project( rush )

if (CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif ()

add_subdirectory( src )

