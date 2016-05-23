#-------------------------------------------------
#
# Project created by QtCreator 2016-05-21T11:14:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ConsoleManager
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    consolewidget.cpp \
    environmentvariablesdialog.cpp \
    associatedportswidget.cpp \
    configuration.cpp

HEADERS  += mainwindow.h \
    consolewidget.h \
    constants.h \
    environmentvariablesdialog.h \
    associatedportswidget.h \
    configuration.h

FORMS    += mainwindow.ui \
    consolewidget.ui \
    environmentvariablesdialog.ui \
    associatedportswidget.ui

RESOURCES += \
    resources.qrc
