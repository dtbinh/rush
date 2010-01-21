project(boost)

include_directories(.)

macro(boost_addlib SOURCE_FILES LIB_NAME)
    file(GLOB LIB_SOURCE_FILES "libs/${LIB_NAME}/src/*.cpp")
    source_group(${LIB_NAME} FILES ${LIB_SOURCE_FILES})
    set(SOURCE_FILES ${SOURCE_FILES} ${LIB_SOURCE_FILES})
endmacro(boost_addlib LIB_NAME)

file(GLOB SOURCE_FILES "libs/detail/*.cpp")
source_group(detail FILES ${SOURCE_FILES})

boost_addlib(${SOURCE_FILES} "system")
boost_addlib(${SOURCE_FILES} "smart_ptr")
boost_addlib(${SOURCE_FILES} "filesystem")

add_library(boost STATIC ${SOURCE_FILES})
