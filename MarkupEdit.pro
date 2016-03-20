TEMPLATE = app

TARGET = MarkupEdit

INCLUDEPATH += ./src

QT += widgets

CONFIG += c++11

# Input
HEADERS += \
	src/mainwindow.h \
	src/markuptextedit.h \
	src/ui_mainwindow.h \

FORMS += \
	src/mainwindow.ui

SOURCES += \
	src/main.cpp \
	src/mainwindow.cpp \
	src/markuptextedit.cpp\

