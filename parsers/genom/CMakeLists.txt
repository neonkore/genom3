
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

add_subdirectory(test)

include_directories(${PROJECT_SOURCE_DIR})

FIND_PACKAGE(BISON)
FIND_PACKAGE(FLEX)

set(GENERATE_PARSER (FLEX_FOUND AND BISON_FOUND AND (BISON_VERSION VERSION_GREATER "2.4")))

if(BISON_VERSION VERSION_GREATER "2.4")
message("Using bison version " ${BISON_VERSION})
endif(BISON_VERSION VERSION_GREATER "2.4")

if(USE_GENERATED_PARSER)
  set(GENERATE_PARSER FALSE)
endif(USE_GENERATED_PARSER)

if(GENERATE_PARSER)
  message("Generating parser using bison version " ${BISON_VERSION})

  BISON_TARGET(GenParser ${PROJECT_SOURCE_DIR}/parsers/genom/parser.yy ${PROJECT_SOURCE_DIR}/parsers/genom/parser.cpp VERBOSE ${PROJECT_SOURCE_DIR}/parsers/genom/log.txt)
  FLEX_TARGET(GenLexer ${PROJECT_SOURCE_DIR}/parsers/genom/lexer.ll ${PROJECT_SOURCE_DIR}/parsers/genom/lexer.cpp)
  ADD_FLEX_BISON_DEPENDENCY(GenLexer GenParser)

  set(genparser_SRCS
      ${FLEX_GenLexer_OUTPUTS}
      ${BISON_GenParser_OUTPUTS}    
      driver.cpp
  )

else(GENERATE_PARSER)
  message("Using generated parser.")
  set(genparser_SRCS
      lexer.cpp
      parser.cpp
      driver.cpp
  )
endif(GENERATE_PARSER)

add_library(genparser ${genparser_SRCS})
target_link_libraries(genparser genutils)
# install(TARGETS genparser LIBRARY DESTINATION lib)
