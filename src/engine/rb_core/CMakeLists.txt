include_directories(. ../../include ../rb_util ../rb_math)

file (GLOB HEADER_FILES "*.h")
file (GLOB SOURCE_FILES "*.cpp")
file(GLOB TEST_FILES "test/*.cpp") 

source_group(src FILES ${HEADER_FILES} ${SOURCE_FILES})
source_group(tests FILES ${TEST_FILES})

add_library(rb_core STATIC ${HEADER_FILES} ${SOURCE_FILES} ${TEST_FILES})

target_link_libraries(rb_core tinyxml unittest++)

if (MSVC)
  add_definitions(/Yu${PCH_FILE_H})
  set_source_files_properties(${SOURCE_FILES}
    PROPERTIES COMPILE_FLAGS /Yu${PCH_FILE_H}
  )
  set_source_files_properties(${PCH_FILE_CPP}
    PROPERTIES COMPILE_FLAGS /Yc${PCH_FILE_H}
  )
endif ()