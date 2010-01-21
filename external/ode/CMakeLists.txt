project(ode)

include_directories(include GIMPACT/include OPCODE ode/src)

file(GLOB ODE_FILES "ode/src/*.cpp" "ode/src/joints/*.cpp" "ode/src/*.c" "ode/src/*.h" "ode/src/joints/*.h")
file(GLOB OPCODE_FILES "OPCODE/*.cpp" "OPCODE/Ice/*.cpp" "OPCODE/*.h" "OPCODE/Ice/*.h")
file(GLOB GIMPACT_FILES "GIMPACT/src/*.cpp" "GIMPACT/include/GIMPACT/*.h")

add_definitions(-DdTRIMESH_ENABLED -DdTRIMESH_OPCODE)

source_group(ode FILES ${ODE_FILES})
source_group(opcode FILES ${OPCODE_FILES})
source_group(gimpact FILES ${GIMPACT_FILES})

add_library(ode STATIC ${ODE_FILES} ${OPCODE_FILES} ${GIMPACT_FILES})
