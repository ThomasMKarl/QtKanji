CXX=
MPICXX=
CXXFLAGS=
INCLUDE_DIR=

## all       : makes build and doc
all:
	make build
	make doc

.PHONY : doc
## doc       : generates html documentation
doc:
	doxygen doc/Doxyfile

.PHONY : tex
## tex       : generates latex documentation
tex: doc
	xelatex doc/tables.tex
	xelatex doc/tables.tex
	mv *.pdf doc/

## build     : compiles application
build:
	qmake -o Makefile_qt
	make  -f Makefile_qt

## clean     : removes object, moc and tex meta files
clean:
	make  -f Makefile_qt clean
	rm -f *.out *.log *.aux *.toc *.tex.bak
	rm -f .qmake.stash

## realclean : removes object files, moc files, tex meta files, compiled executables, documentation and Qt makefile
realclean:
	make clean
	rm -f bin/qtkanji* doc/latex doc/html Makefile_qt


.PHONY : help
## help      : prints help
help : Makefile
	@sed -n 's/^##//p' $<
