project(libvorbis)

include_directories(. include lib ./../libogg/include)

file(GLOB HEADER_FILES "lib/*.h")
file(GLOB SOURCE_FILES "lib/*.c")

source_group(src FILES ${HEADER_FILES} ${SOURCE_FILES})

add_library(libvorbis STATIC ${HEADER_FILES} ${SOURCE_FILES})
