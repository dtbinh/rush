rb_library(rb_extmath)

set(INTERNAL_LIBS 
    rb_math
    rb_util
    )
    
rb_include_libs("${INTERNAL_LIBS}")

target_link_libraries(rb_extmath 
    ${INTERNAL_LIBS}
    )
