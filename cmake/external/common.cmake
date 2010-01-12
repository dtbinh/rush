
# standard library add macro
macro( rb_library LIB_NAME )
    project(${LIB_NAME})
    set( ${LIB_NAME}_INCLUDE_DIR . )
    include_directories( . )
    
    file (GLOB HEADER_FILES "*.h")
    file (GLOB SOURCE_FILES "*.cpp")
    file(GLOB TEST_FILES "test/*.cpp") 

    source_group(src FILES ${HEADER_FILES} ${SOURCE_FILES})
    source_group(tests FILES ${TEST_FILES})

    add_library(${LIB_NAME} STATIC ${HEADER_FILES} ${SOURCE_FILES} ${TEST_FILES})

    if (MSVC)
      add_definitions(/Yu${PCH_FILE_H})
      set_source_files_properties(${SOURCE_FILES}
        PROPERTIES COMPILE_FLAGS /Yu${PCH_FILE_H}
      )
      set_source_files_properties(${PCH_FILE_CPP}
        PROPERTIES COMPILE_FLAGS /Yc${PCH_FILE_H}
      )
    endif ()
endmacro(rb_library)

