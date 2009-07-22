
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

add_subdirectory(test)

FIND_PACKAGE(BISON)
FIND_PACKAGE(FLEX)

include_directories(${PROJECT_SOURCE_DIR})

set(GENERATE_PARSER TRUE)

if(NOT FLEX_FOUND OR NOT BISON_FOUND)
  set(GENERATE_PARSER FALSE)
endif(NOT FLEX_FOUND OR NOT BISON_FOUND)

if(BISON_VERSION VERSION_LESS "2.4")
  message("Found bison version " ${BISON_VERSION} ". Expected version 2.4 at least")
  set(GENERATE_PARSER FALSE)
endif(BISON_VERSION VERSION_LESS "2.4")

if(USE_GENERATED_PARSER)
  set(GENERATE_PARSER FALSE)
endif(USE_GENERATED_PARSER)

if(GENERATE_PARSER)
  message("Generating parser using bison version " ${BISON_VERSION})

  BISON_TARGET(TemplateInfoParser ${PROJECT_SOURCE_DIR}/parsers/template_info/parser.yy ${PROJECT_SOURCE_DIR}/parsers/template_info/parser.cpp)
  FLEX_TARGET(TemplateInfoLexer ${PROJECT_SOURCE_DIR}/parsers/template_info/lexer.ll ${PROJECT_SOURCE_DIR}/parsers/template_info/lexer.cpp)
  ADD_FLEX_BISON_DEPENDENCY(TemplateInfoLexer TemplateInfoParser)

  set(template_info_parser_SRCS
    ${FLEX_TemplateInfoLexer_OUTPUTS}
    ${BISON_TemplateInfoParser_OUTPUTS}    
    templateinterpreter.cpp
  )

else(GENERATE_PARSER)
  message("Using generated parser.")
  set(template_info_parser_SRCS
      lexer.cpp
      parser.cpp
      templateinterpreter.cpp
  )
endif(GENERATE_PARSER)

add_library(template_info_parser ${template_info_parser_SRCS})
target_link_libraries(template_info_parser genutils genom_tcl_binding genom_python_binding)
# install(TARGETS genparser LIBRARY DESTINATION lib)
