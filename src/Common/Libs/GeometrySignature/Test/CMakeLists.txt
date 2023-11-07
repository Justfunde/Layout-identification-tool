set(TEST_TARGET_NAME ${TARGET_NAME}_test)

add_executable(${TEST_TARGET_NAME} main.cpp)
target_link_libraries(${TEST_TARGET_NAME} PRIVATE GeometrySignature GTest::GTest)
gtest_discover_tests(${TEST_TARGET_NAME})