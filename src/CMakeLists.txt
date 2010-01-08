include_directories(
    include
    )
    
set(PCH_FILE_H "precompile.h")
set(PCH_FILE_CPP "precompile.cpp")

add_subdirectory( engine )
add_subdirectory( tools ) 
