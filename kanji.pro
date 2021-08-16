#-------------------------------------------------
#
# Project created by QtCreator 2017-11-08T16:38:26
#
#-------------------------------------------------

QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 6): QT += widgets

TARGET = bin/qtkanji
TEMPLATE = app
RC_ICONS = kanji.ico

HEADERS += \
    include/examplewindow.h \
    include/flashcardwindow.h \
    include/hadamitzkywindow.h \
    include/datahandler.h \
    include/table.h \
    include/mainwindow.h \
    include/helper.h \
    include/headers.h

SOURCES += \
    src/examplewindow.cpp \
    src/flashcardwindow.cpp \
    src/hadamitzkywindow.cpp \
    src/datahandler.cpp \
    src/table.cpp \
    src/mainwindow.cpp \
    src/helper.cpp \
    src/main.cpp
    
QMAKE_CXXFLAGS -= -O2
QMAKE_CXXFLAGS += -std=c++17 -O3 -Wall -Wextra -I include
