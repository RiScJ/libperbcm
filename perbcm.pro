TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += \
		src/ \
		inc/

SOURCES += \
		src/gpio.cpp \
		src/main.cpp

HEADERS += \
		inc/gpio.hpp
