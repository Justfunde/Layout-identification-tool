set(TARGET_NAME Crc32)

add_library(${TARGET_NAME} STATIC Crc32.c)
target_include_directories(${TARGET_NAME} PUBLIC ${SHARED_INCLUDE_DIRS})