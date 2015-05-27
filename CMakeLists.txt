cmake_minimum_required(VERSION 2.6)
project(imagefeaturedetector)
find_package(Qt4 REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

# set(ImageFeatureDetector_SRCS main.cpp windowAbout.cpp windowCaptureWebcam.cpp windowFastRealTime.cpp windowImage.cpp windowMain.cpp windowPreferences.cpp windowStartup.cpp)
file(GLOB ImageFeatureDetector_SRCS *.h *.cpp)

qt4_automoc(${ImageFeatureDetector_SRCS})
add_executable(imagefeaturedetector ${ImageFeatureDetector_SRCS})
target_link_libraries(imagefeaturedetector ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${OpenCV_LIBS})

install(TARGETS imagefeaturedetector RUNTIME DESTINATION bin)
