all:
	@qmake -spec win32-g++ -o Makefile.win32 ../openorbiter.pro DATADIR="."
	@lrelease ../openorbiter.pro
	@make -f Makefile.win32 -j3
	@makensis openorbiter-noqt.nsi
	@makensis openorbiter.nsi

clean:
	@rm -f Makefile.win32
	@rm -fr release
	@rm -f object_script.openorbiter
