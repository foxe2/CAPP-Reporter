#-------------------------------------------------
#
# Project created by QtCreator 2016-11-03T21:11:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CAPP_Reporter
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    TentativeHighlighter.cpp

HEADERS  += MainWindow.hpp \
    main.hpp \
    TentativeHighlighter.hpp

FORMS    += mainwindow.ui
