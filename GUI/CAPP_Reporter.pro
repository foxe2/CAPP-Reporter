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
    ColorText.cpp \
    CourseSelector.cpp \
    ../Algorithm.cpp

HEADERS  += MainWindow.hpp \
    main.hpp \
    ColorText.hpp \
    CourseSelector.hpp \
    ../Algorithm.hpp

FORMS    += mainwindow.ui
