set(TARGET_NAME LayoutIdentificationTool)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

file(GLOB SETTINGS_SOURCES ${CMAKE_CURRENT_LIST_DIR}/Settings/*.cpp)

message(STATUS "SETTINGS SOURCES : '${SETTINGS_SOURCES}'")
set(SOURCES_FILES
    ${SETTINGS_SOURCES}
    MainWindow.cpp
    main.cpp)


set(HEADER_FILES
   MainWindow.hpp)

source_group("Sources Files" FILES ${SOURCES_FILES})
source_group("Headers Files" FILES ${HEADERS_FILES})

add_executable(${TARGET_NAME}  ${SOURCES_FILES} ${HEADERS_FILES}  ${RESOURCES_FILES})
target_link_libraries(${TARGET_NAME} Qt6::Core  Qt6::Gui Qt6::Widgets FileBrowser)