rb_library(rb_rendergl)

set(INTERNAL_LIBS 
    rb_math
    rb_util
    rb_core
    )
    
rb_include_libs("${INTERNAL_LIBS}")

target_link_libraries(rb_rendergl 
    ${INTERNAL_LIBS}
    )

