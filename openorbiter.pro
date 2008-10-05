CONFIG += debug warn_on
FORMS = ui/form_main.ui ui/form_match.ui
HEADERS = src/form_main.h \
 src/form_match.h \
 src/game.h \
 src/map.h \
 src/maploader.h \
 src/match.h \
 src/node.h \
 src/orbiter.h \
 src/physics/circle.h \
 src/physics/object.h \
 src/physics/plane.h \
 src/physics/vector.h \
 src/physics/world.h \
 src/player.h \
 src/scene.h \
 src/view.h
MOC_DIR = build
OBJECTS_DIR = build
QT += core gui opengl xml
SOURCES = src/form_main.cpp \
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
 src/physics/plane.cpp \
 src/physics/world.cpp \
 src/player.cpp \
 src/scene.cpp \
 src/view.cpp
TARGET = bin/openorbiter
TEMPLATE = app
UI_DIR = build
