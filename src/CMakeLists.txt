include_directories( include )
    
set(PCH_FILE_H "precompile.h")
set(PCH_FILE_CPP "precompile.cpp")

add_subdirectory( engine/rb_math )
add_subdirectory( engine/rb_util )
add_subdirectory( engine/rb_core )

add_subdirectory( engine/rb_script_lua )

# add_subdirectory( engine/rb_rendergl )
# add_subdirectory( engine/rb_ui )
# add_subdirectory( engine/rb_draw )

# add_subdirectory( engine/rb_particle )

add_subdirectory( engine/rboot )

add_subdirectory( tools ) 
