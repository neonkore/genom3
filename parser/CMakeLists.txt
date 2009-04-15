
set(CMAKE_MODULE_PATH "../cmake/")

add_subdirectory(test)

FIND_PACKAGE(BISON)
FIND_PACKAGE(FLEX)

include_directories(${PROJECT_SOURCE_DIR}/utils)

BISON_TARGET(GenParser ${PROJECT_SOURCE_DIR}/parser/parser.yy ${PROJECT_SOURCE_DIR}/parser/parser.cpp)
FLEX_TARGET(GenLexer ${PROJECT_SOURCE_DIR}/parser/lexer.ll ${PROJECT_SOURCE_DIR}/parser/lexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(GenLexer GenParser)

set(genparser_SRCS
    ${FLEX_GenLexer_OUTPUTS}
    ${BISON_GenParser_OUTPUTS}    
    driver.cpp
)

add_library(genparser ${genparser_SRCS})
target_link_libraries(genparser genutils)
# install(TARGETS genparser LIBRARY DESTINATION lib)
