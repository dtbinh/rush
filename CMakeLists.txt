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
     "${CMAKE_SOURCE_DIR}/cmake/external"
    )
    
include( common )

set(PCH_FILE_H "precompile.h")
set(PCH_FILE_CPP "precompile.cpp")

add_subdirectory( external )

include_directories( src/include )
    
add_subdirectory( src/engine/rb_math )
add_subdirectory( src/engine/rb_extmath )
add_subdirectory( src/engine/rb_util )
add_subdirectory( src/engine/rb_core )

add_subdirectory( src/engine/rb_script_lua )
add_subdirectory( src/engine/rb_particle )

# add_subdirectory( src/engine/rb_rendergl )
add_subdirectory( src/engine/rb_ui )
add_subdirectory( src/engine/rb_draw )

# add_subdirectory( src/engine/rb_particle )

add_subdirectory( src/engine/rboot )

add_subdirectory( src/tools ) 

