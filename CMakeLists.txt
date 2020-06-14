cmake_minimum_required (VERSION 2.8.11)
project (lightfieldpi)

list(APPEND CMAKE_MODULE_PATH "/usr/local/lib/cmake")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(raspicam REQUIRED)

add_executable (lightfieldpi main.cpp camera_view.cpp main_window.cpp)

target_link_libraries(lightfieldpi Qt5::Widgets ${raspicam_LIBS})