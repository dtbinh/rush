cmake_minimum_required( VERSION 2.6 )

# block the in-source builds
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "In-source build is not allowed.")
endif(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})

# default build is debug
if (CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE "Debug")
endif ()

project( rush )

set( ENGINE_SRC_DIR ${CMAKE_SOURCE_DIR}/src/engine )
set( EXTERNALS_SRC_DIR ${CMAKE_SOURCE_DIR}/external )

set( CMAKE_MODULE_PATH 
     "${CMAKE_SOURCE_DIR}/cmake" 
     "${CMAKE_SOURCE_DIR}/cmake/internal" 
     "${CMAKE_SOURCE_DIR}/cmake/external"
    )
    
include( common )

add_subdirectory( src )
add_subdirectory( external )

