project(unittest++)

include_directories(.)

file(GLOB HEADER_FILES "*.h")
file(GLOB SOURCE_FILES "*.cpp")

source_group(src FILES ${HEADER_FILES} ${SOURCE_FILES})

if (WIN32)
  include_directories("Win32")
  set(SOURCE_FILES ${SOURCE_FILES} Win32/TimeHelpers.cpp)
else ()
  include_directories("Posix")
  set(SOURCE_FILES ${SOURCE_FILES} Posix/SignalTranslator.cpp Posix/TimeHelpers.cpp)
endif(WIN32)



add_library(unittest++ STATIC ${HEADER_FILES} ${SOURCE_FILES} ${TEST_FILES})
