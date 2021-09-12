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
	cd doc
	xelatex tables.tex
	cd latex
	make all
	make clean
	cd ../..

## build     : compiles application
build:
	qmake -o Makefile_qt
	make  -f Makefile_qt

## clean     : removes object, moc and tex meta files
clean:
	make  -f Makefile_qt clean
	rm -p bin/*.out bin/*.log bin/*.aux bin/*.toc bin/*.tex.bak
	rm -p doc/*.out doc/*.log doc/*.aux doc/*.toc doc/*.tex.bak

## realclean : removes object files, moc files, compiled executables, documentation and Qt Makefile
realclean:
	make clean
	rm -f bin/qtkanji* doc/latex doc/html Makefile_qt


.PHONY : help
## help      : prints help
help : Makefile
	@sed -n 's/^##//p' $<
