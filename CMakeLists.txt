cmake_minimum_required(VERSION 2.8)
project(liri-vinyl)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

find_package(Qt5 COMPONENTS Core Gui Sql Quick Qml Svg REQUIRED)
find_package(Taglib REQUIRED)

include_directories(${TAGLIB_INCLUDE_DIRS})

add_subdirectory(src)

