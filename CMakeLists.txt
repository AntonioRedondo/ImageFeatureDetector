# http://cmake.org/runningcmake
# http://doc.qt.io/qt-5/cmake-manual.html

cmake_minimum_required(VERSION 2.8.11)

project(imagefeaturedetector)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)

# set(CMAKE_PREFIX_PATH "/usr/include/x86_64-linux-gnu/qt5/QtCore" "/usr/include/x86_64-linux-gnu/qt5/QtWidgets")

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(OpenCV REQUIRED)

file(GLOB ImageFeatureDetector_SRC *.h *.cpp)
file(GLOB ImageFeatureDetector_UI *.ui)

# http://stackoverflow.com/questions/29968264/linking-and-uic-order-in-a-cmake-qt-project
qt5_wrap_ui(ImageFeatureDetector_UI_H ${ImageFeatureDetector_UI})

add_executable(imagefeaturedetector ${ImageFeatureDetector_SRC} ${ImageFeatureDetector_UI_H})

target_link_libraries(imagefeaturedetector Qt5::Core Qt5::Widgets ${OpenCV_LIBS})
