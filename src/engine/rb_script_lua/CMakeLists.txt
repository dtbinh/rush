rb_library(rb_script_lua)

set(INTERNAL_LIBS 
    rb_math
    rb_util
    rb_core
    )

include(lua)
    
rb_include_libs("${INTERNAL_LIBS}")

target_link_libraries(rb_script_lua 
    ${INTERNAL_LIBS}
    lua
    )
