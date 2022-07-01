TEMPLATE = app
TARGET = logical-frogger

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    main.cpp \
    screens/StartScreen.cpp \
    screens/gameoptionsscreen.cpp \
    state-machine/FroggerStateMachine.cpp

HEADERS += \
    screens/StartScreen.h \
    screens/gameoptionsscreen.h \
    state-machine/FroggerStateMachine.hpp
