# project(GenParser)

set(CMAKE_MODULE_PATH "../cmake/")

FIND_PACKAGE(BISON)
FIND_PACKAGE(FLEX)

# include_directories(lib)

FLEX_TARGET(GenLexer ${PROJECT_SOURCE_DIR}/parser/lexer.ll ${PROJECT_BINARY_DIR}/lexer.cpp)
BISON_TARGET(GenParser ${PROJECT_SOURCE_DIR}/parser/parser.yy ${PROJECT_BINARY_DIR}/parser.cpp)
ADD_FLEX_BISON_DEPENDENCY(GenLexer GenParser)

set(genparser_SRCS
    ast.cpp
    ${LEX_GenLexer_OUTPUTS}
    ${BISON_GenParser_OUTPUTS}    
    driver.cpp
)

add_library(genparser SHARED ${genparser_SRCS})
# install(TARGETS genparser LIBRARY DESTINATION lib)
