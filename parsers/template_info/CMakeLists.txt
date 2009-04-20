
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

add_subdirectory(test)

FIND_PACKAGE(BISON)
FIND_PACKAGE(FLEX)

include_directories(${PROJECT_SOURCE_DIR})

BISON_TARGET(TemplateInfoParser ${PROJECT_SOURCE_DIR}/parsers/template_info/parser.yy ${PROJECT_SOURCE_DIR}/parsers/template_info/parser.cpp)
FLEX_TARGET(TemplateInfoLexer ${PROJECT_SOURCE_DIR}/parsers/template_info/lexer.ll ${PROJECT_SOURCE_DIR}/parsers/template_info/lexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(TemplateInfoLexer TemplateInfoParser)

set(template_info_parser_SRCS
    ${FLEX_TemplateInfoLexer_OUTPUTS}
    ${BISON_TemplateInfoParser_OUTPUTS}    
    templateinterpreter.cpp
)

add_library(template_info_parser ${template_info_parser_SRCS})
target_link_libraries(template_info_parser genutils)
# install(TARGETS genparser LIBRARY DESTINATION lib)
