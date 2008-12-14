confpaths.h: Makefile
	echo "#define PKGDATADIR \"$(pkgdatadir)\"" > $@

bin_PROGRAMS = openorbiter
openorbiter_SOURCES = collider.cpp deathzone.cpp input.cpp map.cpp match.cpp \
		node.cpp object.cpp openorbiter.cpp orbiter.cpp player.cpp renderer.cpp \
		solid.cpp lfontrenderer.cpp Font.cpp FontData.cpp

renderer.cpp: confpaths.h

# set the include path found by configure
INCLUDES= $(all_includes)


# the library search path.
#openorbiter_LDFLAGS = $(all_libraries) $(LIBSDL_RPATH)
openorbiter_CXXFLAGS = $(LIBSDL_CFLAGS)
openorbiter_LDADD = $(top_builddir)/src/timer/libmercury_timer.a \
	$(top_builddir)/src/math/libmath.a -lSDL_ttf -lGLU -lGL $(LIBSDL_LIBS)
noinst_HEADERS = node.h orbiter.h map.h object.h match.h player.h deathzone.h \
		renderer.h input.h solid.h collider.h lfontrenderer.h Font.h FontData.h Types.h
SUBDIRS = math timer common

DISTCLEANFILES = confpaths.h
