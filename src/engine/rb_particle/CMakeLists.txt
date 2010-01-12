rb_library(rb_particle)

include(rb_core)
include(rb_util)
include(rb_math) 

include(unittest++)

target_link_libraries(rb_particle 
    rb_core rb_util rb_math 
    unittest++)
