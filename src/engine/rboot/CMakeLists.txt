include_directories(.)

include(unittest++)

set(INTERNAL_LIBS
    rb_math
    rb_util
    rb_core
    rb_particle
    rb_script_lua
    rb_draw
    rb_ui
    rb_scene
    )

if(WIN32)
    set(INTERNAL_LIBS ${INTERNAL_LIBS} rb_render9)
else(WIN32)
    set(INTERNAL_LIBS ${INTERNAL_LIBS} rb_rendergl)
endif(WIN32)

rb_include_libs("${INTERNAL_LIBS}")

# unit test files
set(SOURCE_FILES
    main.cpp
    rboot.cpp
    precompile.cpp
    modelviewer.cpp
    testdriver.cpp)

set(HEADER_FILES
  precompile.h
  modelviewer.h
  testdriver.h)
set_source_files_properties(${HEADER_FILES} PROPERTIES HEADER_FILE_ONLY 1)

source_group(src FILES ${SOURCE_FILES} ${HEADER_FILES})

foreach(LIB ${INTERNAL_LIBS})
    rb_add_tests("${SOURCE_FILES}" "${LIB}")
endforeach(LIB ${INTERNAL_LIBS})

rb_set_pch("${SOURCE_FILES}")

if(WIN32)
    add_executable(rboot WIN32 ${SOURCE_FILES} ${HEADER_FILES})
else(WIN32)
    add_executable(rboot ${SOURCE_FILES} ${HEADER_FILES})
endif(WIN32)

target_link_libraries(rboot
    ${INTERNAL_LIBS}
    unittest++
    )

#add_custom_command(TARGET rboot POST_BUILD
#                   COMMAND ${CMAKE_CURRENT_BINARY_DIR} ARGS --unittests)


