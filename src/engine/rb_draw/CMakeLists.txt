rb_library(rb_draw)

set(INTERNAL_LIBS 
    rb_core
    rb_math
    rb_extmath
    rb_util
    )
    
rb_include_libs("${INTERNAL_LIBS}")

target_link_libraries(rb_draw 
    ${INTERNAL_LIBS}
    )
