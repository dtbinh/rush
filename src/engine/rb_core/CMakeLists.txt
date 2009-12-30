include_directories(. ../../include ../rb_util ../rb_math)

file (GLOB HEADER_FILES "*.h")
file (GLOB SOURCE_FILES "*.cpp")

add_library(rb_core STATIC ${HEADER_FILES} ${SOURCE_FILES})