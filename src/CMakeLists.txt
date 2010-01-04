include_directories(
    include
    "${CMAKE_CURRENT_SOURCE_DIR}/../external/unittest-cpp-1.4/src"
    )
    
set(PCH_FILE_H "precompile.h")
set(PCH_FILE_CPP "precompile.cpp")

add_subdirectory( engine )
add_subdirectory( tools ) 
