rb_library(rb_core)

set(INTERNAL_LIBS 
    rb_math
    rb_util
    )
    
rb_include_libs("${INTERNAL_LIBS}")
include(tinyxml)

target_link_libraries(rb_core 
    ${INTERNAL_LIBS}
    tinyxml)
