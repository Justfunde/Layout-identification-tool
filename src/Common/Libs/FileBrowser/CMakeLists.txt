set(TARGET_NAME FileBrowser)

add_library(${TARGET_NAME} STATIC FileBrowser.cpp)
target_link_libraries(${TARGET_NAME} PUBLIC Qt6::Core Qt6::Widgets)
target_include_directories(${TARGET_NAME} PUBLIC ${SHARED_INCLUDE_DIRS})