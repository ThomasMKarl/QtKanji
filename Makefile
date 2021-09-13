DOXYGEN=doxygen
TEX=xelatex
MAKE=make
QMAKE=qmake
#MAKE=E:\Programme\MinGW\bin\make.exe
#QMAKE=E:\Programme\Qt\Static\5.12.10\bin\qmake.exe

## all       : makes build and doc
all:
	$(MAKE) build
	$(MAKE) doc

.PHONY : doc
## doc       : generates html documentation
doc:
	$(DOXYGEN) doc/Doxyfile

.PHONY : tex
## tex       : generates latex documentation
tex: doc
	$(TEX) doc/tables.tex
	$(TEX) doc/tables.tex
	mv *.pdf doc/

## build     : compiles application
build:
	$(QMAKE) -o Makefile_qt
	$(MAKE)  -f Makefile_qt

## clean     : removes object, moc and tex meta files
clean:
	$(MAKE)  -f Makefile_qt clean
	rm -f *.out *.log *.aux *.toc *.tex.bak
	rm -f .qmake.stash

## realclean : removes object files, moc files, tex meta files, compiled executables, documentation and Qt makefile
realclean:
	$(MAKE) clean
	rm -f bin/qtkanji* doc/latex doc/html Makefile_qt


.PHONY : help
## help      : prints help
help : Makefile
	@sed -n 's/^##//p' $<
