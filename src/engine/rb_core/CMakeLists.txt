rb_library(rb_core)

include(rb_util)
include(rb_math) # FIXME

include(unittest++)
include(tinyxml)

target_link_libraries(rb_core tinyxml unittest++)
