
FIND_PACKAGE(TCL)
FIND_PACKAGE(Boost)

include_directories(${PROJECT_SOURCE_DIR}/parser ${TCL_INCLUDE_PATH} ${Boost_INCLUDE_DIRS})

set(genom_tcl_binding_SRCS
  cpptcl.cc
  tclinterpreter.cpp
  test.cpp
)

add_executable(genom_tcl_binding_test ${genom_tcl_binding_SRCS})
target_link_libraries(genom_tcl_binding_test ${TCL_LIBRARY} genparser)