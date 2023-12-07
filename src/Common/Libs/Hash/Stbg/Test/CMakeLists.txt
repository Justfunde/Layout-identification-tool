set(TEST_TARGET_NAME Stbg_test)

add_executable(${TEST_TARGET_NAME} main.cpp)
target_link_libraries(${TEST_TARGET_NAME} PRIVATE Stbg GTest::GTest)
gtest_discover_tests(${TEST_TARGET_NAME})