TEMPLATE = app
TARGET = logical-frogger

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    FroggerWindow.cpp \
    main.cpp \
    screens/GameOptionsScreen.cpp \
    screens/StartScreen.cpp \
    state-machine/FroggerStateMachine.cpp

HEADERS += \
    FroggerWindow.h \
    GameSettings.h \
    screens/GameOptionsScreen.h \
    screens/StartScreen.h \
    state-machine/FroggerStateMachine.hpp
