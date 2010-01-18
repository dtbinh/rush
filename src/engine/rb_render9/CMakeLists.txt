rb_library(rb_render9)

set(INTERNAL_LIBS 
    rb_math
    rb_util
    rb_core
    )
    
rb_include_libs("${INTERNAL_LIBS}")

target_link_libraries(rb_render9 
    ${INTERNAL_LIBS}
    )
