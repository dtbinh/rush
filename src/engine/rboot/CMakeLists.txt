
set(SOURCE_FILES "test.cpp")

add_executable(rboot ${SOURCE_FILES})

target_link_libraries(rboot 
    rb_math
    
    unittest++
    )
