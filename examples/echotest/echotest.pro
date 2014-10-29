QT = gui widgets

TEMPLATE = app

CONFIG += link_pkgconfig
PKGCONFIG += qwebsocket

HEADERS += mainwindow.h
FORMS += mainwindow.ui
SOURCES += main.cpp mainwindow.cpp
