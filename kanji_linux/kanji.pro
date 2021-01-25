#-------------------------------------------------
#
# Project created by QtCreator 2017-11-08T16:38:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = kanji
TEMPLATE = app


HEADERS += \
    ../sources/examplewindow.h \
    ../sources/flashcard.h \
    ../sources/functions.h \
    ../sources/mainwindow.h \
    ../sources/screen.h

SOURCES += \
    ../sources/examplewindow.cpp \
    ../sources/flashcard.cpp \
    ../sources/functions.cpp \
    ../sources/main.cpp \
    ../sources/mainwindow.cpp
    
QMAKE_CXXFLAGS -= -O2
QMAKE_CXXFLAGS += -std=c++11 -O3 -Wall -Wextra


