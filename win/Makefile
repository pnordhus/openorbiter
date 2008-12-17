VERSION = "0.0"


all:
	@qmake -spec win32-g++ -o Makefile.win32 ../openorbiter.pro DATADIR="."
	@lrelease ../openorbiter.pro
	@make -f Makefile.win32 -j3
	@unix2dos -n ../ChangeLog ChangeLog.txt
	@unix2dos -n ../README.win ReadMe.txt
	@makensis -Dversion=$(VERSION) openorbiter.nsi

clean:
	@rm -f Makefile.win32
	@rm -fr release
	@rm -f object_script.openorbiter
	@rm -f ChangeLog.txt
