include_directories(. ../../include)

#find_package(UnitTest++ REQUIRED)

file(GLOB HEADER_FILES "*.h")
file(GLOB SOURCE_FILES "*.cpp")
file(GLOB TEST_FILES "test/*.cpp")

source_group(src FILES ${HEADER_FILES} ${SOURCE_FILES})
source_group(tests FILES ${TEST_FILES})

add_library(rb_math STATIC ${HEADER_FILES} ${SOURCE_FILES} ${TEST_FILES})

if (MSVC)
  add_definitions(/Yu${PCH_FILE_NAME})
  set_source_files_properties(${SOURCE_FILES}
    PROPERTIES COMPILE_FLAGS /Yu${PCH_FILE_NAME}
  )
  set_source_files_properties("stdafx.cpp"
    PROPERTIES COMPILE_FLAGS /Yc${PCH_FILE_NAME}
  )
endif ()
