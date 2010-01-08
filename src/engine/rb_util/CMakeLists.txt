include_directories(. ../../include)

file (GLOB HEADER_FILES "*.h")
file (GLOB SOURCE_FILES "*.cpp")

add_library(rb_util STATIC ${HEADER_FILES} ${SOURCE_FILES})

if (MSVC)
  add_definitions(/Yu${PCH_FILE_H})
  set_source_files_properties(${SOURCE_FILES}
    PROPERTIES COMPILE_FLAGS /Yu${PCH_FILE_H}
  )
  set_source_files_properties(${PCH_FILE_CPP}
    PROPERTIES COMPILE_FLAGS /Yc${PCH_FILE_H}
  )
endif ()