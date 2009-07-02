FIND_PACKAGE(SWIG REQUIRED)
FIND_PACKAGE(TCL)

include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/parsers/genom ${TCL_INCLUDE_PATH})

SET(SWIG_FLAGS "-c++")

MACRO(SWIG_TARGET Name Input)
  SET(SWIG_TARGET_usage "SWIG_TARGET(<Name> <Input>")
  GET_FILENAME_COMPONENT(InputBaseName ${Input} NAME_WE)
  GET_FILENAME_COMPONENT(InputName ${Input} NAME)
  GET_FILENAME_COMPONENT(InputPath ${Input} PATH)

  SET(SWIG_OUTPUTS ${InputPath}/${InputBaseName}_wrap.cxx)
  ADD_CUSTOM_COMMAND(OUTPUT ${SWIG_OUTPUTS}
    COMMAND ${SWIG_EXECUTABLE} ${SWIG_FLAGS} -tcl8 ${Input}
    DEPENDS ${Input}
    COMMENT "[SWIG] Generating wrappers from ${Input}"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

  SET(SWIG_${Name}_OUTPUTS ${SWIG_OUTPUTS})
  SET(SWIG_${Name}_INPUT ${Input})
ENDMACRO(SWIG_TARGET)

SWIG_TARGET(GENOM_TCL ${PROJECT_SOURCE_DIR}/bindings/swig/genom.i)

set(genom_tcl_binding_SRCS
  tclinterpreter.cpp
  tclwrappers.cpp
)

set_source_files_properties(tclwrappers.cpp OBJECT_DEPENDS ${SWIG_GENOM_TCL_OUTPUTS})

add_library(genom_tcl_binding ${genom_tcl_binding_SRCS})
target_link_libraries(genom_tcl_binding ${TCL_LIBRARY} genutils)

# Test app

set(genom_tcl_test_SRCS
  test.cpp
)

add_executable(genom_tcl_binding_test ${genom_tcl_test_SRCS})
target_link_libraries(genom_tcl_binding_test genparser genom_tcl_binding)

