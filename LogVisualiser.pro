#-------------------------------------------------
#
# Project created by QtCreator 2016-07-08T23:13:25
#
#-------------------------------------------------

QT       += core gui

TARGET = LogVisualiser
DESTDIR = build
TEMPLATE = app
INCLUDES += ../cl2/trunk

INCLUDEPATH += . ../cl2/trunk
	      
LIBS += build/cl2.lib

SOURCES += main.cpp\
        app.cpp \
		qcustomplot/qcustomplot.cpp
		

HEADERS  += app.h qcustomplot/qcustomplot.h

FORMS    += app.ui
