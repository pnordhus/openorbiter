add_subdirectory(physics)
add_subdirectory(translations)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(QT_USE_QTXML true)
include(${QT_USE_FILE})

set(SOURCES
	bouncer.cpp
	bouncer.h
	bouncerdef.cpp
	bouncerdef.h
	form_main.cpp
	form_main.h
	form_match.cpp
	form_match.h
	game.cpp
	game.h
	main.cpp
	map.cpp
	map.h
	mapdef.cpp
	mapdef.h
	maploader.cpp
	maploader.h
	match.cpp
	match.h
	node.cpp
	node.h
	orbiter.cpp
	orbiter.h
	player.cpp
	player.h
	rendermanager.cpp
	rendermanager.h
	scene.cpp
	scene.h
	view.cpp
	view.h
)

set(MOC_SOURCES
	form_main.h
	form_match.h
	game.h
	match.h
	orbiter.h
	rendermanager.h
	scene.h
	view.h
)

set(UIC_SOURCES
    form_about.ui
    form_main.ui
    form_match.ui
)

qt4_wrap_cpp(MOC_OUTPUT ${MOC_SOURCES})
qt4_wrap_ui(UIC_OUTPUT ${UIC_SOURCES})

add_executable(openorbiter ${SOURCES} ${MOC_OUTPUT} ${UIC_OUTPUT})
target_link_libraries(openorbiter physics ${QT_LIBRARIES})

install(TARGETS openorbiter DESTINATION bin)
