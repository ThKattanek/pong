TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

LIBS += -lsfml-system -lsfml-window -lsfml-graphics

include(deployment.pri)
qtcAddDeployment()

