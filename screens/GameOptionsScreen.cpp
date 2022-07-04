#include "GameOptionsScreen.h"
#include <cstdlib>
#include <sstream>
#include <time.h>
#include <iomanip>
#include <iostream>

#include <QVBoxLayout>
#include <QLabel>
#include <QSpacerItem>
#include <QGridLayout>
#include <QPushButton>
#include <QScrollArea>

const std::string GameOptionsScreen::screenName = "Game Options Screen";

GameOptionsScreen::GameOptionsScreen(QWidget* parent, int numLevels) : QWidget(parent) {
    std::srand(std::time(0));
    // Automatically set random map.
    for (int i=0;i<numLevels;i++) {
        currentSetting.pos.push_back(std::rand()*1./RAND_MAX);
        // 10 to 22 or -22 to -10.
        currentSetting.vel.push_back(((std::rand()%13)+10)*((std::rand()%2)*2 - 1));
    }
    setObjectName(screenName.c_str());
    // GUI elements
    initGUI();
    setInactive();
}

void GameOptionsScreen::setActive() {
    setVisible(true);
    isActive=true;
}

void GameOptionsScreen::setInactive() {
    setVisible(false);
    isActive=false;
}

void GameOptionsScreen::changeVelocity(int level, int newVel) {
    currentSetting.vel[level] = newVel;
    // set GUI
    guiLevels[level].posDisp->setText(std::to_string(newVel).c_str());
}

void GameOptionsScreen::changePosition(int level, float newPos) {
    // Set GUI and value
    currentSetting.pos[level] = newPos;
    std::stringstream newPosStream;

    newPosStream << std::fixed << std::setprecision(5) << newPos;
    guiLevels[level].posDisp->setText(newPosStream.str().c_str());
}

void GameOptionsScreen::changeNumLevels(int newNumLevels) {
    if (newNumLevels < numLevels) {
        // Disable gui elements
        for (int i=newNumLevels;i<numLevels;i++)
            disableLevel(i);
    }
    else if (newNumLevels > numLevels) {
        // Enable gui elements
        for (int i=numLevels;i<newNumLevels;i++)
            activateLevel(i);
    }
    numLevels=newNumLevels;
}

void GameOptionsScreen::disableLevel(int level) {
    QList<QWidget*> levelGUIElements;
    // Find all GUI elements
    levelGUIElements.append(guiLevels[level].velSetter->findChildren<QWidget*>());
    levelGUIElements.append(guiLevels[level].posSetter->findChildren<QWidget*>());
    levelGUIElements.append(guiLevels[level].velDisp->findChildren<QWidget*>());
    levelGUIElements.append(guiLevels[level].posDisp->findChildren<QWidget*>());
    levelGUIElements.append(guiLevels[level].velSetter);
    levelGUIElements.append(guiLevels[level].posSetter);
    levelGUIElements.append(guiLevels[level].velDisp);
    levelGUIElements.append(guiLevels[level].posDisp);
    for (auto i : levelGUIElements) i->setVisible(false);
}

void GameOptionsScreen::activateLevel(int level) {
    QList<QWidget*> levelGUIElements;
    // Find all GUI elements
    levelGUIElements.append(guiLevels[level].velSetter->findChildren<QWidget*>());
    levelGUIElements.append(guiLevels[level].posSetter->findChildren<QWidget*>());
    levelGUIElements.append(guiLevels[level].velDisp->findChildren<QWidget*>());
    levelGUIElements.append(guiLevels[level].posDisp->findChildren<QWidget*>());
    levelGUIElements.append(guiLevels[level].velSetter);
    levelGUIElements.append(guiLevels[level].posSetter);
    levelGUIElements.append(guiLevels[level].velDisp);
    levelGUIElements.append(guiLevels[level].posDisp);
    for (auto i : levelGUIElements) i->setVisible(true);
}

void GameOptionsScreen::initGUI() {
    setFixedSize(parentWidget()->size());

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setFixedSize(size());

    QWidget* wholeContainer = new QWidget(scrollArea);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QLabel *title = new  QLabel(this);
    title->setText("Game Options");

    QGridLayout *optionsLayout = new QGridLayout;

    /*for (int i=0;i<maxNumLevels;i++) {
        struct LevelGUIData levelGUI(wholeContainer);
        optionsLayout->addWidget(levelGUI.velSetter, 2*i, 0, 1, 1);
        optionsLayout->addWidget(levelGUI.posSetter, 2*i+1, 0, 1, 1);
        optionsLayout->addWidget(levelGUI.velDisp, 2*i, 1, 1, 1);
        optionsLayout->addWidget(levelGUI.posDisp, 2*i+1, 1, 1, 1);
        guiLevels.push_back(levelGUI);
    }*/

    mainLayout->addWidget(title);
    mainLayout->addLayout(optionsLayout);

    wholeContainer->setLayout(mainLayout);
    scrollArea->setWidget(wholeContainer);
}
