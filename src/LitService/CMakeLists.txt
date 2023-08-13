set(TARGET_NAME LitSerive)

set(SOURCES_LIST 
   ServiceMain.cpp
   Daemon.cpp
   Types.cpp
   )

set(HEADERS_LIST
   Daemon.hpp
   Types.hpp
   )



add_executable(${TARGET_NAME} ${SOURCES_LIST} ${HEADERS_LIST})
set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 23) 