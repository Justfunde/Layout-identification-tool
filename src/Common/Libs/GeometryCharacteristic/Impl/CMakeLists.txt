sыet(TARGET_NAME GeometryCharacteristic)

add_library(${TARGET_NAME} STATIC GeometryCharacteristic.cpp)
target_include_directories(${TARGET_NAME} PUBLIC ${SHARED_INCLUDE_DIRS})