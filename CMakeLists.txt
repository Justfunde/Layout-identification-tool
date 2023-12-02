cmake_minimum_required(VERSION 3.19)

project(LAYOUT_IDENTIFICATION_TOOL LANGUAGES CXX)

#enable_testing()
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(PROJECT_DIR ${CMAKE_CURRENT_LIST_DIR})

#Initialization of common build parameters
set(BUILD_TYPE_LIST "Debug" "Release")
set(OUTPUT_DIR "${CMAKE_CURRENT_LIST_DIR}/Output")

set(OUTPUT_DEBUG_DIR "{CMAKE_CURRENT_LIST_DIR}/Debug")
set(OUTPUT_RELEASE_DIR "{CMAKE_CURRENT_LIST_DIR}/Release")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")

#env

find_package(Qt5 COMPONENTS Core Gui Widgets Sql REQUIRED)
find_package(Boost REQUIRED)

enable_testing()
find_package(GTest REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

include(DoxygenModule)

#generate_doxygen(${PROJECT_DIR})


add_subdirectory(src)