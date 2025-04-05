set(TARGET_NAME Stbg)

add_library(${TARGET_NAME} STATIC Stbg.cpp)
target_include_directories(${TARGET_NAME} PUBLIC ${SHARED_INCLUDE_DIRS})

#add_subdirectory(Test)