set(TEST_TARGET_NAME GeometryCharacteristic_test)


add_executable(${TEST_TARGET_NAME} main.cpp)
target_link_libraries(${TEST_TARGET_NAME} PRIVATE  GTest::GTest ${Boost_LIBRARIES} LayoutReader)
target_compile_definitions(${TEST_TARGET_NAME} PRIVATE "LAYOUTS_DIR=\"${CMAKE_CURRENT_LIST_DIR}/TestLayouts\"")
target_compile_definitions(${TEST_TARGET_NAME} PRIVATE "LAYOUTS_DATA_OUT_DIR=\"${CMAKE_CURRENT_LIST_DIR}/OutDir\"")
target_include_directories(${TEST_TARGET_NAME} PUBLIC ${SHARED_INCLUDE_DIRS})
gtest_discover_tests(${TEST_TARGET_NAME})