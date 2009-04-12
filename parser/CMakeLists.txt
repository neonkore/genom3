
set(CMAKE_MODULE_PATH "../cmake/")

FIND_PACKAGE(BISON)
FIND_PACKAGE(FLEX)

include_directories(${PROJECT_SOURCE_DIR}/parser ${PROJECT_BINARY_DIR})

BISON_TARGET(GenParser ${PROJECT_SOURCE_DIR}/parser/parser.yy parser/parser.cpp)
FLEX_TARGET(GenLexer ${PROJECT_SOURCE_DIR}/parser/lexer.ll parser/lexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(GenLexer GenParser)

set(genparser_SRCS
    ast.cpp
    ${FLEX_GenLexer_OUTPUTS}
    ${BISON_GenParser_OUTPUTS}    
    driver.cpp
)

add_library(genparser SHARED ${genparser_SRCS})
# install(TARGETS genparser LIBRARY DESTINATION lib)
