rb_library(rb_script_lua)

include(rb_core)
include(rb_util)
include(rb_math) 

include(unittest++)
include(lua)

target_link_libraries(rb_script_lua 
    rb_core rb_util rb_math 
    lua unittest++)
