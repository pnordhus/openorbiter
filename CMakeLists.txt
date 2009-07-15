cmake_minimum_required(VERSION 2.6)
project(openorbiter)

find_package(Qt4 4.4.0 REQUIRED)

set(QT_USE_QTSVG true CACHE BOOL "Build with SVG support")
set(QT_USE_QTOPENGL true CACHE BOOL "Build with OpenGL support")
set(QT_USE_QTXML true)
include(${QT_USE_FILE})

set(CMAKE_DATA_PATH ${CMAKE_INSTALL_PREFIX}/share CACHE PATH "Directory for shared files, 'openorbiter' will be appended automatically")
set(DATA_PATH ${CMAKE_DATA_PATH}/openorbiter)

configure_file(src/config.h.in config.h)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(SOURCES
	src/bouncer.cpp
	src/bouncer.h
	src/bouncerdef.cpp
	src/bouncerdef.h
	src/defs.h
	src/form_main.cpp
	src/form_main.h
	src/form_match.cpp
	src/form_match.h
	src/game.cpp
	src/game.h
	src/main.cpp
	src/map.cpp
	src/map.h
	src/mapdef.cpp
	src/mapdef.h
	src/maploader.cpp
	src/maploader.h
	src/match.cpp
	src/match.h
	src/node.cpp
	src/node.h
	src/orbiter.cpp
	src/orbiter.h
	src/player.cpp
	src/player.h
	src/rendermanager.cpp
	src/rendermanager.h
	src/scene.cpp
	src/scene.h
	src/view.cpp
	src/view.h
	
	src/physics/circle.cpp
	src/physics/circle.h
	src/physics/object.cpp
	src/physics/object.h
	src/physics/rect.cpp
	src/physics/rect.h
	src/physics/vector.h
	src/physics/world.cpp
	src/physics/world.h
)

set(MOC_SOURCES
	src/form_main.h
	src/form_match.h
	src/game.h
	src/match.h
	src/orbiter.h
	src/rendermanager.h
	src/scene.h
	src/view.h
	
	src/physics/object.h
)

set(UIC_SOURCES
	ui/form_about.ui
	ui/form_main.ui
	ui/form_match.ui
)

source_group(src src/*)
source_group(src\\physics src/physics/*)
source_group(ui ui/*)

qt4_wrap_cpp(MOC_SOURCES ${MOC_SOURCES})
qt4_wrap_ui(UIC_SOURCES ${UIC_SOURCES})
qt4_add_resources(QRC_SOURCES res/openorbiter.qrc)
qt4_add_translation(QM_SOURCES data/translations/openorbiter_de.ts)

add_executable(openorbiter ${SOURCES} ${UIC_SOURCES} ${MOC_SOURCES} ${QRC_SOURCES} ${QM_SOURCES})
target_link_libraries(openorbiter ${QT_LIBRARIES})

install(TARGETS openorbiter DESTINATION bin)
install(DIRECTORY "data/gfx" DESTINATION "${DATA_PATH}" PATTERN ".svn" EXCLUDE)
install(DIRECTORY "data/maps" DESTINATION "${DATA_PATH}" PATTERN ".svn" EXCLUDE)
install(FILES ${QM_SOURCES} DESTINATION "${DATA_PATH}/translations")
