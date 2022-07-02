#include "FroggerWindow.h"

FroggerWindow::FroggerWindow(QWidget* parent) : QWidget(parent) {
    setFixedSize(512, 512);
    startScreen = new StartScreen(this);
    gameOptionsScreen = new GameOptionsScreen(this);
}
