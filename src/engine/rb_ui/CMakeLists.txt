rb_library(rb_ui)

set(INTERNAL_LIBS 
    rb_core
    rb_math
    rb_util
    )
    
rb_include_libs("${INTERNAL_LIBS}")

target_link_libraries(rb_ui 
    ${INTERNAL_LIBS}
    )
