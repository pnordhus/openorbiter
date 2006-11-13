!include(build/configure.pri) {
	error("Please run ./configure first!")
}

QT += xml
MOC_DIR = build
UI_DIR = build
OBJECTS_DIR = build
TARGET = openorbiter
DESTDIR = bin
TEMPLATE = app

!win32 {
	QMAKE_CXXFLAGS += -fno-exceptions
}

release {
	CONFIG += silent
}

win32 {
	CONFIG += rtti
}

FORMS += \
	ui/form_creatematch.ui \
	ui/form_main.ui \
	ui/form_selectkey.ui \
	ui/form_selectmap.ui \
	ui/form_settings.ui


HEADERS +=	\
	src/config.h \
	src/defs.h \
	src/form_creatematch.h \
	src/form_main.h \
	src/form_selectkey.h \
	src/form_selectmap.h \
	src/form_settings.h \
	src/game.h \
	src/gamestats.h \
	src/graphicsscene.h \
	src/graphicsview.h \
	src/map.h \
	src/match.h \
	src/matchstats.h \
	src/node.h \
	src/openorbiter.h \
	src/orbiter.h \
	src/player.h \
	src/spawnpoint.h \
	src/vector.h


SOURCES +=	\
	src/config.cpp \
	src/form_creatematch.cpp \
	src/form_main.cpp \
	src/form_selectkey.cpp \
	src/form_selectmap.cpp \
	src/form_settings.cpp \
	src/game.cpp \
	src/gamestats.cpp \
	src/graphicsscene.cpp \
	src/graphicsview.cpp \
	src/main.cpp \
	src/map.cpp \
	src/match.cpp \
	src/matchstats.cpp \
	src/node.cpp \
	src/openorbiter.cpp \
	src/orbiter.cpp \
	src/player.cpp \
	src/spawnpoint.cpp \
	src/vector.cpp

!exists(build/configure.h) {
	error("Please run ./configure again!")
}


target.path = $${BINDIR}/

datamaps.path = $${DATADIR}/maps/
datamaps.files = \
	data/maps/classic.xml \
	data/maps/oomap.dtd

datagfx.path = $${DATADIR}/gfx/
datagfx.files = \
	data/gfx/blue.svg \
	data/gfx/cyan.svg \
	data/gfx/fuchsia.svg \
	data/gfx/green.svg \
	data/gfx/node.svg \
	data/gfx/red.svg \
	data/gfx/yellow.svg

INSTALLS += target datamaps datagfx
