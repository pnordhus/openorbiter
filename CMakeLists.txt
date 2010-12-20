cmake_minimum_required(VERSION 2.6)
project(openorbiter)

set(OPENORBITER_VERSION "0.5-pre")

find_package(Qt4 4.4.0 REQUIRED)

set(QT_USE_QTSVG true CACHE BOOL "Build with SVG support")
set(QT_USE_QTOPENGL true CACHE BOOL "Build with OpenGL support")

set(CMAKE_LANGUAGES de en CACHE STRING "Languages")
set(CMAKE_DATA_PATH ${CMAKE_INSTALL_PREFIX}/share CACHE PATH "Directory for shared files, 'openorbiter' will be appended automatically")
set(DATA_PATH ${CMAKE_DATA_PATH}/openorbiter)

configure_file(src/config.h.in src/config.h)
include_directories(${CMAKE_BINARY_DIR}/src)

add_subdirectory(src)

install(DIRECTORY "data/gfx" DESTINATION "${DATA_PATH}")
install(DIRECTORY "data/maps" DESTINATION "${DATA_PATH}")
