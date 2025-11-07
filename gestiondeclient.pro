QT       += core gui sql
QT       += widgets

CONFIG += c++17

TEMPLATE = app
TARGET = gestiondeclient

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    Login.cpp \
    connection.cpp \
    createur.cpp \
    gestioncreateur.cpp \
    prediction.cpp

HEADERS += \
    mainwindow.h \
    Login.h \
    connection.h \
    createur.h \
