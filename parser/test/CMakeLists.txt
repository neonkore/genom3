
ENABLE_TESTING()

include_directories(${PROJECT_SOURCE_DIR}/parser)

set(lexertest_SRCS
    lexertest_main.cpp)

ADD_EXECUTABLE(lexertest ${lexertest_SRCS})

# target_link_libraries(lexertest ${PROJECT_BINARY_DIR}/parser/libgenparser.so)
target_link_libraries(lexertest genparser)

ADD_TEST(testLexer ${EXECUTABLE_OUTPUT_PATH}/lexertest)