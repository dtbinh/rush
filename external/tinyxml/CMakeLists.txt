project(tinyxml)

include_directories(.)

file(GLOB HEADER_FILES "*.h")
file(GLOB SOURCE_FILES "*.cpp")

source_group(src FILES ${HEADER_FILES} ${SOURCE_FILES})

add_library(tinyxml STATIC ${HEADER_FILES} ${SOURCE_FILES})
