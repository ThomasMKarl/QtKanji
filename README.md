# QtKanji

A kanji training tool based on Qt for Linux and Windows, now conatining 540 of the most important signs

##Dependencies

pre-built binaries are available at releases

- Windows: none, build is static

- Linux: Qt5

- (optional) doxygen for code documentation

- (optional) xelatex for LaTeX tables

##Compile

- install Gnu Make with a C++17 compliant compiler

- get rapidjson

- install Qt >= 5

- insert correct commands in <i>Makefile</i>

- insert correct path to rapidjson in <i>kanji.pro</i>

- type <i>make help</i> in root folder:

 all       : makes build and doc

 doc       : generates html documentation

 tex       : generates latex documentation

 build     : compiles application

 clean     : removes object, moc and tex meta files

 realclean : removes object files, moc files, tex meta files, compiled executables, documentation and Qt makefile

 help      : prints help

the last one needs <i>sed</i> in the path variable

- if needed, copy the qtkanji binary to bin/

##Useage

- execute <i>qtkanji</i> in folder <i>bin</i>

Database files <i>*.dat</i> / <i>*.json</i> must reside in the same folder in which the <i>qtkanji</i> binary is executed.

##Under construction

- print function for flashcards

- examples for more than 317-460

- cardbox function for examples
