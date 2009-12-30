
file (GLOB HEADER_FILES "*.h")
file (GLOB SOURCE_FILES "*.cpp")

add_library(rb_math STATIC ${HEADER_FILES} ${SOURCE_FILES})
