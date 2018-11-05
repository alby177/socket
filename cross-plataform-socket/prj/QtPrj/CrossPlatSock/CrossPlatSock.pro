TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    ../../../src/Server.cpp \
    ../../../src/main.cpp \
    ../../../src/Client.cpp

HEADERS += \
    ../../../src/Server.h \
    ../../../src/Client.h

LIBS += -lws2_32

Debug:DESTDIR = ../../../bin/debug
Debug:OBJECTS_DIR = ../../../bin/debug/.obj
Debug:MOC_DIR = ../../../bin/debug/.moc
Debug:RCC_DIR = ../../../bin/debug/.rcc
Debug:UI_DIR = ../../../bin/debug/.ui
