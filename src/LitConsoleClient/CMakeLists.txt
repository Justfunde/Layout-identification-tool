set(TARGET_NAME LitConsoleClient)



set(SOURCES_LIST
   main.cpp)

add_executable(${TARGET_NAME} ${SOURCES_LIST})
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR} ${SHARED_INCLUDE_DIRS})
target_link_libraries(${TARGET_NAME} PRIVATE LayoutReader Crc32 GeometrySignature)