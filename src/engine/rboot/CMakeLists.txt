include_directories(.)

include(unittest++)
include(rb_math)
include(rb_util)
include(rb_core)
#include(rb_particle)

set(SOURCE_FILES test.cpp precompile.h precompile.cpp)

add_executable(rboot ${SOURCE_FILES})

target_link_libraries(rboot 
    rb_math
    rb_util
    rb_core
    rb_particle
    rb_script_lua
    
    unittest++
    )
    
#add_custom_command(TARGET rboot POST_BUILD
#                   COMMAND ${CMAKE_CURRENT_BINARY_DIR} ARGS --unittests)
    
    
