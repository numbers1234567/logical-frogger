#include <screens/StartScreen.h>

StartScreen::StartScreen(QWidget* parent) : QWidget(parent) {
    QHBoxLayout *buttonContainer = new QHBoxLayout(this);

    startButton = new QPushButton("Start", this);
    helpButton = new QPushButton("Help", this);

    buttonContainer->addWidget(startButton);
    buttonContainer->addWidget(helpButton);

    setObjectName("Start Screen");
}

StartScreen::~StartScreen() {
}
