project(sqlite)

include_directories(.)

file(GLOB HEADER_FILES "*.h")
file(GLOB SOURCE_FILES "*.c")

source_group(src FILES ${HEADER_FILES} ${SOURCE_FILES})

add_library(sqlite STATIC ${HEADER_FILES} ${SOURCE_FILES})
