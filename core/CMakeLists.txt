
include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/utils)

set(genom_core_SRCS
  test.cpp
)

add_executable(genom ${genom_core_SRCS})
target_link_libraries(genom genutils template_info_parser genparser genom_python_binding genom_tcl_binding)