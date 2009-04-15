
FIND_PACKAGE(TCL)
FIND_PACKAGE(Boost)

include_directories(${PROJECT_SOURCE_DIR}/utils ${PROJECT_SOURCE_DIR}/parser ${TCL_INCLUDE_PATH} ${Boost_INCLUDE_DIRS})

# Lib

set(genom_tcl_binding_SRCS
  cpptcl.cc
  tclinterpreter.cpp
)

add_library(gen_tcl_binding ${genom_tcl_binding_SRCS})
target_link_libraries(gen_tcl_binding ${TCL_LIBRARY} genutils)

# Test app

set(genom_tcl_test_SRCS
  test.cpp
)

add_executable(genom_tcl_binding_test ${genom_tcl_test_SRCS})
target_link_libraries(genom_tcl_binding_test genparser gen_tcl_binding)