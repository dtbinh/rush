cmake_minimum_required( VERSION 2.6 )

project( rush )
add_subdirectory( src )

set(PCH_FILE_NAME "stdafx.h")

include_directories(./src/include)