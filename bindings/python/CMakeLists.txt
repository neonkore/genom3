
FIND_PACKAGE(PythonLibs)
FIND_PACKAGE(Boost COMPONENTS python)

include_directories(${PROJECT_SOURCE_DIR}/utils ${PROJECT_SOURCE_DIR}/parser ${PYTHON_INCLUDE_PATH} ${Boost_INCLUDE_DIRS})

# Lib

set(genom_python_binding_SRCS
  pythoninterpreter.cpp
)

add_library(genom_python_binding ${genom_python_binding_SRCS})
target_link_libraries(genom_python_binding ${PYTHON_LIBRARIES} ${Boost_LIBRARIES} genutils)

# Test app

set(genom_python_test_SRCS
  test.cpp
)

add_executable(genom_python_binding_test ${genom_python_test_SRCS})
target_link_libraries(genom_python_binding_test genparser genom_python_binding)