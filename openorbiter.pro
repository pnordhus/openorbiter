isEmpty(PREFIX) {
	PREFIX =  /usr/local
}

isEmpty(DATADIR) {
	DATADIR =  $$PREFIX/share/openorbiter
}

TEMPLATE = app
CONFIG += silent warn_on
DEFINES += OO_DATADIR=\"\\\"$$DATADIR\\\"\"

QT += core gui xml
INSTALLS += \
 maps \
 target

!no-opengl {
	QT += opengl
}

!no-svg {
	QT += svg
	INSTALLS += gfx
}

win32-g++ {
	CONFIG -= debug_and_release
	CONFIG -= debug_and_release_target
	
	OBJECTS_DIR = release
	MOC_DIR = release
	UI_DIR = release
	TARGET = release/bin/openorbiter
} else {
	CONFIG += debug_and_release
	
	build_pass:CONFIG(debug, debug|release) {
		OBJECTS_DIR = build/debug
		MOC_DIR = build/debug
		UI_DIR = build/debug
		TARGET = bin/openorbiter_debug
	} else {
		OBJECTS_DIR = build/release
		MOC_DIR = build/release
		UI_DIR = build/release
		TARGET = bin/openorbiter
	}
}


HEADERS = \
 src/bouncer.h \
 src/bouncerdef.h \
 src/form_main.h \
 src/form_match.h \
 src/game.h \
 src/map.h \
 src/maploader.h \
 src/match.h \
 src/node.h \
 src/orbiter.h \
 src/physics/circle.h \
 src/physics/object.h \
 src/physics/rect.h \
 src/physics/vector.h \
 src/physics/world.h \
 src/player.h \
 src/rendermanager.h \
 src/scene.h \
 src/view.h

FORMS = \
 ui/form_main.ui \
 ui/form_match.ui

SOURCES = src/bouncer.cpp \
 src/bouncerdef.cpp \
 src/form_main.cpp \
 src/form_match.cpp \
 src/game.cpp \
 src/main.cpp \
 src/map.cpp \
 src/maploader.cpp \
 src/match.cpp \
 src/node.cpp \
 src/orbiter.cpp \
 src/physics/circle.cpp \
 src/physics/object.cpp \
 src/physics/rect.cpp \
 src/physics/world.cpp \
 src/player.cpp \
 src/rendermanager.cpp \
 src/scene.cpp \
 src/view.cpp

maps.path = $$DATADIR/maps
maps.files += \
 data/maps/cage.xml \
 data/maps/dipper.xml \
 data/maps/double.xml \
 data/maps/single.xml \
 data/maps/triangle.xml

gfx.path = $$DATADIR/gfx
gfx.files += \
 data/gfx/blue.svg \
 data/gfx/cyan.svg \
 data/gfx/green.svg \
 data/gfx/magenta.svg \
 data/gfx/node.svg \
 data/gfx/red.svg \
 data/gfx/yellow.svg

DISTFILES += COPYING ChangeLog README

target.path = $$PREFIX/bin
