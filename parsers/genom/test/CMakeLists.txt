
ENABLE_TESTING()

include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/parsers/genom ${PROJECT_SOURCE_DIR}/utils ${Boost_INCLUDE_DIRS})

set(lexertest_SRCS
    lexertest_main.cpp)

ADD_EXECUTABLE(lexertest ${lexertest_SRCS})

# target_link_libraries(lexertest ${PROJECT_BINARY_DIR}/parser/libgenparser.so)
target_link_libraries(lexertest genparser)

ADD_TEST(testLexerComponent ${EXECUTABLE_OUTPUT_PATH}/lexertest "testComponent")
ADD_TEST(testLexerTask ${EXECUTABLE_OUTPUT_PATH}/lexertest "testTask")
ADD_TEST(testLexerPorts ${EXECUTABLE_OUTPUT_PATH}/lexertest "testPorts")
ADD_TEST(testFail ${EXECUTABLE_OUTPUT_PATH}/lexertest "testFil")

FIND_PACKAGE(Boost COMPONENTS unit_test_framework)

set(parser_unittest_SRCS
    parser_unit_test.cpp)

ADD_EXECUTABLE(parser_unittest ${parser_unittest_SRCS})

# target_link_libraries(lexertest ${PROJECT_BINARY_DIR}/parser/libgenparser.so)
target_link_libraries(parser_unittest genparser ${Boost_LIBRARIES})

