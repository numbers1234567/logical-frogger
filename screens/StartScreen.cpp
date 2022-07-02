#include <screens/StartScreen.h>
#include <screens/GameOptionsScreen.h>

#include <iostream>

const std::string StartScreen::screenName="Start Screen";

StartScreen::StartScreen(QWidget* parent) : QWidget(parent) {
    QHBoxLayout *buttonLayout = new QHBoxLayout(this);

    startButton = new QPushButton("Start", this);
    helpButton = new QPushButton("Help", this);

    buttonLayout->addWidget(startButton);
    buttonLayout->addWidget(helpButton);

    connect(startButton, SIGNAL(clicked()), this, SLOT(onStartButtonPressed()));

    setObjectName(screenName.c_str());

    //setInactive();
}

StartScreen::~StartScreen() {
}

void StartScreen::onStartButtonPressed() {
    emit startButtonPressed();
    //std::cout << " Clicked start  " << std::endl;
    GameOptionsScreen* gameOptionsScreen = parentWidget()->findChild<GameOptionsScreen*>(gameOptionsScreen->screenName.c_str());
    // Might be able to spruce this up later with some sort of transition
    setInactive();
    gameOptionsScreen->setActive();
}

void StartScreen::setInactive() {
    setVisible(false);
    QList<QWidget*> children = findChildren<QWidget*>();
    for (auto i : children) i->setVisible(false);
    isActive=false;
}

void StartScreen::setActive() {
    setVisible(true);
    QList<QWidget*> children = findChildren<QWidget*>();
    for (auto i : children) i->setVisible(true);
    isActive=true;
}
