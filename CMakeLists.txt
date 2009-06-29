cmake_minimum_required(VERSION 2.6)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(UseDoxygen)

add_subdirectory(utils)
add_subdirectory(parsers)
add_subdirectory(bindings)
add_subdirectory(core)
