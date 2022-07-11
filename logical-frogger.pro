TEMPLATE = app
TARGET = logical-frogger

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    FroggerGameDisplay.cpp \
    FroggerWindow.cpp \
    main.cpp \
    screens/GameOptionsScreen.cpp \
    screens/GameScreen.cpp \
    screens/StartScreen.cpp \
    state-machine/FroggerStateMachine.cpp

HEADERS += \
    FroggerGameDisplay.h \
    FroggerWindow.h \
    GameSettings.h \
    screens/GameOptionsScreen.h \
    screens/GameScreen.h \
    screens/StartScreen.h \
    state-machine/FroggerStateMachine.hpp
