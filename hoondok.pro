#-------------------------------------------------
#
# Project created by QtCreator 2023-10-05T09:07:05
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += qaxcontainer
TARGET = hoondok
TEMPLATE = app

win32: RC_ICONS = $$PWD/bin/images/app.ico
DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += c++11

SOURCES += \
        main.cpp \
	mainwindow.cpp \
        database.cpp \
	qregexphighlighter.cpp \
	func.cpp

HEADERS += \
	mainwindow.h \
        database.h \
	qregexphighlighter.h \
	func.h \
	gconst.h

FORMS += \
        mainwindow.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    bin/img.qrc \
    bin/qss.qrc
