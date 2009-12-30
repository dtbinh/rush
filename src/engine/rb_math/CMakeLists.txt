include_directories(. ../../include)

file (GLOB HEADER_FILES "*.h")
file (GLOB SOURCE_FILES "*.cpp")

add_library(rb_math STATIC ${HEADER_FILES} ${SOURCE_FILES})

if (MSVC)
  add_definitions(/Yu${PCH_FILE_NAME})
  set_source_files_properties(${SOURCE_FILES}
    PROPERTIES COMPILE_FLAGS /Yu${PCH_FILE_NAME} 
  )
  set_source_files_properties("stdafx.cpp"
    PROPERTIES COMPILE_FLAGS /Yc${PCH_FILE_NAME} 
  )
endif ()
