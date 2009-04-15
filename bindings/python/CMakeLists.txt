
FIND_PACKAGE(PythonLibs)
FIND_PACKAGE(Boost COMPONENTS python)

include_directories(${PROJECT_SOURCE_DIR}/parser ${PYTHON_INCLUDE_PATH} ${Boost_INCLUDE_DIRS})

set(genom_python_binding_test_SRCS
  pythoninterpreter.cpp
  test.cpp
)

add_executable(genom_python_binding_test ${genom_python_binding_test_SRCS})
target_link_libraries(genom_python_binding_test ${PYTHON_LIBRARIES} ${Boost_LIBRARIES} genparser)