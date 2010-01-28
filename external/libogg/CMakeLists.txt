project(libogg)

include_directories(include)

file(GLOB HEADER_FILES "src/*.h")
file(GLOB SOURCE_FILES "src/*.c")

source_group(src FILES ${HEADER_FILES} ${SOURCE_FILES})

add_library(libogg STATIC ${HEADER_FILES} ${SOURCE_FILES})
