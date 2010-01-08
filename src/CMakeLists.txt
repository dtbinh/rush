include_directories(
    include
    "${CMAKE_CURRENT_SOURCE_DIR}/../external/unittest++"
    )
    
set(PCH_FILE_H "precompile.h")
set(PCH_FILE_CPP "precompile.cpp")

add_subdirectory( engine )
add_subdirectory( tools ) 
