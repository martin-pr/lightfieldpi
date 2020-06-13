cmake_minimum_required (VERSION 2.8.11)
project (lightfieldpi)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable (lightfieldpi main.cpp)

target_link_libraries(lightfieldpi Qt5::Widgets)