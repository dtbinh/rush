project(erl_interface)

include_directories(include src/connect src/misc src/epmd src/auxdir)

file(GLOB SOURCE_FILES 
  "src/connect/*.c" 
  "src/decode/*.c"
  "src/encode/*.c"
  "src/epmd/*.c"
  "src/legacy/*.c"
  "src/misc/*.c"
  "src/prog/*.c"
  "src/registry/*.c"
)

if(NOT WIN32)
    ADD_DEFINITIONS(-DHAVE_SOCKLEN_T=1 -DHAVE_SYS_TIME_H=1 -DHAVE_STRERROR=1)
endif(NOT WIN32)

add_library(erl_interface STATIC ${SOURCE_FILES})
