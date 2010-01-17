
macro(rb_set_pch SOURCE_FILES)
    if(MSVC)
      add_definitions(/Yu${PCH_FILE_H})
      set_source_files_properties(${SOURCE_FILES}
        PROPERTIES COMPILE_FLAGS /Yu${PCH_FILE_H}
      )
      set_source_files_properties(${PCH_FILE_CPP}
        PROPERTIES COMPILE_FLAGS /Yc${PCH_FILE_H}
      )
    endif(MSVC)
endmacro(rb_set_pch SOURCE_FILES)

# standard library add macro
macro(rb_library LIB_NAME )
    project(${LIB_NAME})
    include_directories(.)
    
    file(GLOB HEADER_FILES "*.h")
    file(GLOB SOURCE_FILES "*.cpp")
    source_group(src FILES ${HEADER_FILES} ${SOURCE_FILES})

    set(LIB_FILES ${HEADER_FILES} ${SOURCE_FILES})

    if(WIN32)
        file(GLOB WIN32_HEADER_FILES "win32/*.h")
	file(GLOB WIN32_SOURCE_FILES "win32/*.cpp")
	source_group(src\\win32 FILES ${WIN32_HEADER_FILES} ${WIN32_SOURCE_FILES})
	set(LIB_FILES ${HEADER_FILES} ${SOURCE_FILES})
    endif(WIN32)

    add_library(${LIB_NAME} STATIC ${LIB_FILES})

    rb_set_pch("${SOURCE_FILES}")
endmacro(rb_library)

macro(rb_add_tests SOURCE_FILES LIB_NAME)
    file(GLOB TEST_FILES "${ENGINE_SRC_DIR}/${LIB_NAME}/test/*.cpp") 
    source_group(tests\\${LIB_NAME} FILES ${TEST_FILES})
    set(SOURCE_FILES ${SOURCE_FILES} ${TEST_FILES})
endmacro(rb_add_tests)

macro(rb_include_libs LIBS)
    foreach(CLIB ${LIBS})
        include_directories( "${ENGINE_SRC_DIR}/${CLIB}" )
    endforeach(CLIB ${LIBS})
endmacro(rb_include_libs)
