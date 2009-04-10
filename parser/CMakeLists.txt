project(GenParser)

FIND_PACKAGE(BISON)
FIND_PACKAGE(FLEX)

include_directories(lib)

BISON_TARGET(GenParser parser.yy ${PROJECT_BINARY_DIR}/parser.cpp)
FLEX_TARGET(GenLexer lexer.ll ${PROJECT_BINARY_DIR}/lexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(GenLexer GenParser)

set(genparser_SRCS
    driver.cpp
    ast.cpp
    ${LEX_GenLexer_OUTPUTS}
    ${BISON_GenParser_OUTPUTS}    
)

add_library(genparser SHARED ${parser_SRCS})
install(TARGETS genparser DESTINATION ${PLUGIN_INSTALL_DIR})
