
ENABLE_TESTING()

include_directories(${PROJECT_SOURCE_DIR}/parsers/template_info ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/utils)

set(template_info_test_SRCS
    template_info_test.cpp)

ADD_EXECUTABLE(template_info_test ${template_info_test_SRCS})

# target_link_libraries(lexertest ${PROJECT_BINARY_DIR}/parser/libgenparser.so)
target_link_libraries(template_info_test template_info_parser genparser)

# ADD_TEST(testLexerComponent ${EXECUTABLE_OUTPUT_PATH}/lexertest "testComponent")
# ADD_TEST(testLexerTask ${EXECUTABLE_OUTPUT_PATH}/lexertest "testTask")
# ADD_TEST(testLexerPorts ${EXECUTABLE_OUTPUT_PATH}/lexertest "testPorts")
# ADD_TEST(testFail ${EXECUTABLE_OUTPUT_PATH}/lexertest "testFil")