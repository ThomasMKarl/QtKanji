#-------------------------------------------------
#
# Project created by QtCreator 2017-11-08T16:38:26
#
#-------------------------------------------------

QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 6): QT += widgets

TARGET = bin/linux/qtkanji
TEMPLATE = app

HEADERS += \
    include/examplewindow.h \
    include/flashcard.h \
    include/functions.h \
    include/datahandler.h \
    include/mainwindow.h \
    include/headers.h

SOURCES += \
    src/examplewindow.cpp \
    src/flashcard.cpp \
    src/functions.cpp \
    src/datahandler.cpp \
    src/mainwindow.cpp \
    src/main.cpp
    
QMAKE_CXXFLAGS -= -O2
QMAKE_CXXFLAGS += -std=c++17 -O3 -Wall -Wextra -I include
