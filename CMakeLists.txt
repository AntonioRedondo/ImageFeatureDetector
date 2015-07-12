## http://cmake.org/runningcmake
## http://doc.qt.io/qt-5/cmake-manual.html

cmake_minimum_required(VERSION 2.8.11)

project(imagefeaturedetector)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin) # http://stackoverflow.com/questions/18826789/cmake-output-build-directory
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTORCC ON) # Not working. To much time invested on this.
# set(CMAKE_AUTOUIC ON) # Not useful. It causes troubles with subdirectories, and with the linking order. Better use qt5_wrap_ui()


## http://stackoverflow.com/questions/11783932/how-to-add-linker-or-compile-flag-in-cmake-file
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lz")


## http://www.cmake.org/Wiki/CMake:How_To_Find_Libraries
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(OpenCV REQUIRED)
# find_package(Threads REQUIRED) # http://stackoverflow.com/questions/1620918/cmake-and-libpthread (if not 'libpthread.so.0: DSO missing from command line' error will be thrown at linking time)
# find_package(ZLIB REQUIRED) # http://stackoverflow.com/questions/6173915/need-help-trying-to-make-cmake-find-third-party-libraries


## http://stackoverflow.com/questions/3201154/cmake-automatically-add-all-files-in-a-folder-to-a-target
file(GLOB ImageFeatureDetector_SRC *.h *.cpp)
file(GLOB ImageFeatureDetector_UI ${CMAKE_SOURCE_DIR}/ui/*.ui)
# file(GLOB ImageFeatureDetector_A ${CMAKE_SOURCE_DIR}/libs/*.a) # http://stackoverflow.com/questions/14077611/how-do-i-tell-cmake-to-link-in-a-static-library-in-the-source-directory


## http://stackoverflow.com/questions/29968264/linking-and-uic-order-in-a-cmake-qt-project
qt5_wrap_ui(ImageFeatureDetector_UI_H ${ImageFeatureDetector_UI})

add_executable(imagefeaturedetector ${ImageFeatureDetector_SRC} ${ImageFeatureDetector_UI_H})

target_link_libraries(imagefeaturedetector Qt5::Core Qt5::Widgets ${OpenCV_LIBS})
# target_link_libraries(imagefeaturedetector Qt5::Core Qt5::Widgets ${ImageFeatureDetector_A} ${CMAKE_THREAD_LIBS_INIT} -lz -ldl)


## http://stackoverflow.com/questions/13429656/cmake-how-to-copy-contents-of-a-directory-into-build-directory-after-make
add_custom_command(TARGET imagefeaturedetector POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/icons $<TARGET_FILE_DIR:imagefeaturedetector>/icons)


## http://www.cmake.org/Wiki/CMake:Install_Commands
## Icons won't be shown because the loading path is relative to the command shell path at the moment of calling ./imagefeaturedetector
# install (TARGETS imagefeaturedetector DESTINATION bin)
# file(GLOB icons ${CMAKE_SOURCE_DIR}/icons/*)
# install (FILES ${icons} DESTINATION bin/icons)



## Deb package generator. IT DOESN'T WORK YET! ##

# http://www.cmake.org/cmake/help/v3.0/module/CPackDeb.html
# https://wiki.debian.org/IntroDebianPackaging
# http://stackoverflow.com/questions/7249440/how-to-build-debian-package-with-cpack-to-execute-setup-py
# http://stackoverflow.com/questions/10086615/cmake-cpack-debian-packages
# set(CPACK_PACKAGE_NAME "image-feature-detector")
# set(CPACK_PACKAGE_VENDOR "Antonio Redondo")
# set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "IFD is a C++ Qt GUI desktop program to calculate with OpenCV Harris, FAST, SIFT and SURF image features")
# set(CPACK_PACKAGE_VERSION "2.0")
# set(CPACK_PACKAGE_VERSION_MAJOR "2")
# set(CPACK_PACKAGE_VERSION_MINOR "0")
# set(CPACK_PACKAGE_VERSION_PATCH "0")
# set(CPACK_SYSTEM_NAME "amd64")
# set(CPACK_GENERATOR "DEB")
# set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Antonio Redondo")
# set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON) # http://www.guyrutenberg.com/2012/07/19/auto-detect-dependencies-when-building-debs-using-cmake/
## set(CPACK_DEBIAN_PACKAGE_DEPENDS "libQt5Core (>= 5.4.0), libQt5Widgets (>= 5.4.0), libQt5Gui (>= 5.4.0)")
# set(CPACK_SET_DESTDIR "ON")
# 
# include(CPack)
#
## Deb package generator END ##
