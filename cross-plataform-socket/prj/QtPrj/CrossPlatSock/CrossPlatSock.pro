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

Debug:DESTDIR = ../../../bin/debug/Qt
Debug:OBJECTS_DIR = ../../../bin/debug/Qt
Debug:MOC_DIR = ../../../bin/debug/Qt
Debug:RCC_DIR = ../../../bin/debug/Qt
Debug:UI_DIR = ../../../bin/debug/Qt
