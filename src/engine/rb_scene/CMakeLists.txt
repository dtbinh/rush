rb_library(rb_scene)

set(INTERNAL_LIBS 
    rb_math
    rb_util
    rb_core
    rb_ui
    rb_draw
    )
    
rb_include_libs("${INTERNAL_LIBS}")

target_link_libraries(rb_scene 
    ${INTERNAL_LIBS}
    )
