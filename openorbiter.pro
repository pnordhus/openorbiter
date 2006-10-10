QT += uitool xml
MOC_DIR = build
UI_DIR = build
OBJECTS_DIR = build
TARGET = openorbiter
DESTDIR = bin
TEMPLATE = app


FORMS += \
	ui/form_creatematch.ui \
	ui/form_main.ui \
	ui/form_selectkey.ui \
	ui/form_selectmap.ui


HEADERS +=	\
	src/config.h \
	src/config_parser.h \
	src/form_creatematch.h \
	src/form_main.h \
	src/form_selectkey.h \
	src/form_selectmap.h \
	src/frame_map.h \
	src/frame_mapback.h \
	src/game.h \
	src/gamestats.h \
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
	src/config_parser.cpp \
	src/form_creatematch.cpp \
	src/form_main.cpp \
	src/form_selectkey.cpp \
	src/form_selectmap.cpp \
	src/frame_map.cpp \
	src/frame_mapback.cpp \
	src/game.cpp \
	src/gamestats.cpp \
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


!include(build/configure.pri) {
	error("Please run ./configure first!")
}

!exists(build/configure.h) {
	error("Please run ./configure again!")
}


target.path = $${BINDIR}/

datamaps.path = $${DATADIR}/maps/
datamaps.files = \
	data/maps/classic.xml

INSTALLS += target datamaps
