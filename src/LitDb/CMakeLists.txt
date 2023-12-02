set(TARGET_NAME LitDb)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


file(GLOB_RECURSE HEADERS_LIST *.h *.hpp) 
file(GLOB_RECURSE SOURCES_LIST *.cpp) 



add_executable(${TARGET_NAME} ${SOURCES_LIST} ${HEADERS_LIST} )
target_link_libraries(${TARGET_NAME} PRIVATE Qt5::Core Qt5::Widgets Qt5::Gui Qt5::Sql)