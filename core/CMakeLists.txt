
include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/utils)

set(genom_core_SRCS
  main.cpp
)

add_executable(genom3 ${genom_core_SRCS})
target_link_libraries(genom3 genutils template_info_parser genparser genom_python_binding genom_tcl_binding)

set(genom_core_SRCS
  test.cpp
)

add_executable(test_gen ${genom_core_SRCS})
target_link_libraries(test_gen genutils template_info_parser genparser genom_python_binding genom_tcl_binding)