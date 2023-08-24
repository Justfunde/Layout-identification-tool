set(TARGET_NAME LitDb)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


file(GLOB_RECURSE HEADERS_LIST *.h *.hpp) 
file(GLOB_RECURSE SOURCES_LIST *.cpp) 



add_executable(${TARGET_NAME} ${SOURCES_LIST} ${HEADERS_LIST} )
target_link_libraries(${TARGET_NAME} PRIVATE Qt6::Core Qt6::Widgets Qt6::Gui Qt6::Sql)