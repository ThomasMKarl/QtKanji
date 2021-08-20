CXX=
MPICXX=
CXXFLAGS=
INCLUDE_DIR=

## all       : makes build and doc
all:
	make build
	make doc

.PHONY : doc
## doc       : generates documentation
doc:
	doxygen doc/Doxyfile

## build     : compiles application
build:
	qmake -o Makefile_qt
	make  -f Makefile_qt

## clean     : removes object files and moc files
clean:
	make  -f Makefile_qt clean

## realclean : removes object files, moc files, compiled executables, documentation and Qt Makefile
realclean:
	make clean
	rm -f bin/qtkanji* doc/latex doc/html Makefile_qt


.PHONY : help
## help      : prints help
help : Makefile
	@sed -n 's/^##//p' $<
