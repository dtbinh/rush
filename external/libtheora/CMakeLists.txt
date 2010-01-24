project(libtheora)

include_directories(.)

file(GLOB HEADER_FILES "lib/*.h")
file(GLOB SOURCE_FILES "lib/*.c")

source_group(src FILES ${HEADER_FILES} ${SOURCE_FILES})

add_library(libtheora STATIC ${HEADER_FILES} ${SOURCE_FILES})
