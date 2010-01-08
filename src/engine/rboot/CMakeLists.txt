
set(SOURCE_FILES "test.cpp")

add_executable(rboot ${SOURCE_FILES})

target_link_libraries(rboot 
    rb_math
    rb_util
    rb_core
    unittest++
    )
    
#add_custom_command(TARGET rboot POST_BUILD
#                   COMMAND ${CMAKE_CURRENT_BINARY_DIR} ARGS --unittests)
    
    
