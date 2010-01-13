include_directories(.)

include(unittest++)

set(INTERNAL_LIBS 
    rb_math
    rb_util
    rb_core
    rb_particle
    rb_script_lua
    rb_draw
    rb_ui
    )
    
rb_include_libs("${INTERNAL_LIBS}")

# unit test files
set(SOURCE_FILES test.cpp precompile.h precompile.cpp)
source_group(src FILES ${SOURCE_FILES})

foreach(LIB ${INTERNAL_LIBS})
    rb_add_tests("${SOURCE_FILES}" "${LIB}")
endforeach(LIB ${INTERNAL_LIBS})

rb_set_pch("${SOURCE_FILES}")

add_executable(rboot ${SOURCE_FILES})

target_link_libraries(rboot 
    ${INTERNAL_LIBS}
    unittest++
    )
    
#add_custom_command(TARGET rboot POST_BUILD
#                   COMMAND ${CMAKE_CURRENT_BINARY_DIR} ARGS --unittests)
    
    
