set(TARGET_NAME LayoutAnalyzer)

file(GLOB_RECURSE SOURCE_LIST *.cpp)
file(GLOB_RECURSE HEADER_LIST *.h *.hpp)


add_library(${TARGET_NAME} STATIC ${SOURCE_LIST} ${HEADER_LIST})
target_include_directories(${TARGET_NAME} PUBLIC ${SHARED_INCLUDE_DIRS})