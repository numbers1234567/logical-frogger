#include "GameOptionsScreen.h"
#include <cstdlib>
#include <sstream>
#include <time.h>
#include <iomanip>
#include <iostream>

#include <QVBoxLayout>
#include <QGridLayout>
#include <QFormLayout>

#include <QScrollArea>

#include <QLabel>
#include <QSpacerItem>
#include <QPushButton>

const std::string GameOptionsScreen::screenName = "Game Options Screen";

GameOptionsScreen::GameOptionsScreen(QWidget* parent, int numLevels) : QWidget(parent) {
    initRandMap();
    setObjectName(screenName.c_str());
    // GUI elements
    initGUI();

    changeNumLevels(numLevels);
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

void GameOptionsScreen::changeNumLevels(int newNumLevels) {
    for (int i=0;i<newNumLevels;i++) activateLevel(i);
    for (int i=newNumLevels;i<maxNumLevels;i++) disableLevel(i);
    numLevels=newNumLevels;
}

void GameOptionsScreen::startGame() {
    GameSetting levelSettings;
    // Populate levelSettings velocity and position based on slider positions
    for (int i=0;i<numLevels;i++) {
        levelSettings.pos.push_back(guiLevels[i].posSetter->tickPosition()*1./128);
        levelSettings.vel.push_back(guiLevels[i].velSetter->tickPosition());
    }
    setInactive();
    // Set game screen as active and provide level settings.
}

void GameOptionsScreen::displayHelp() {
    // To-do later maybe, but not a priority right now
}

void GameOptionsScreen::disableLevel(int level) {
    QList<QWidget*> levelGUIElements;
    // Find all GUI elements
    levelGUIElements.append(guiLevels[level].velSetter);
    levelGUIElements.append(guiLevels[level].posSetter);
    levelGUIElements.append(guiLevels[level].velDisp);
    levelGUIElements.append(guiLevels[level].posDisp);
    levelGUIElements.append(guiLevels[level].levelDisp);
    for (auto i : levelGUIElements) i->hide();
}

void GameOptionsScreen::activateLevel(int level) {
    QList<QWidget*> levelGUIElements;
    // Find all GUI elements
    levelGUIElements.append(guiLevels[level].velSetter);
    levelGUIElements.append(guiLevels[level].posSetter);
    levelGUIElements.append(guiLevels[level].velDisp);
    levelGUIElements.append(guiLevels[level].posDisp);
    levelGUIElements.append(guiLevels[level].levelDisp);
    for (auto i : levelGUIElements) i->show();
}

void GameOptionsScreen::initRandMap() {
    std::srand(std::time(0));
    // Automatically set random map.
    for (int i=0;i<maxNumLevels;i++) {
        initialSetting.pos.push_back((std::rand()%127)*1./128);
        // 5 to 15 or -15 to -5.
        initialSetting.vel.push_back(((std::rand()%11)+5)*((std::rand()%2)*2 - 1));
    }
}

void GameOptionsScreen::initGUI() {
    setFixedSize(parentWidget()->size());

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout;
    // Top layout
    QHBoxLayout *topLayout = new QHBoxLayout;

    QLabel *title = new  QLabel(this);
    title->setText("Game Options");

    QPushButton *startGameButton = new QPushButton("Start Game", this);
    connect(startGameButton, SIGNAL(clicked()), this, SLOT(startGame()));
    QPushButton *helpButton = new QPushButton("Help", this);

    topLayout->addWidget(title);
    topLayout->addWidget(startGameButton);
    topLayout->addWidget(helpButton);
    // Top layout end
    // numLevels
    QSlider *numLevelsSetter = new QSlider(Qt::Horizontal, this);
    numLevelsSetter->setMinimum(minNumLevels);
    numLevelsSetter->setMaximum(maxNumLevels);
    connect(numLevelsSetter, SIGNAL(valueChanged(int)), this, SLOT(changeNumLevels(int)));
    // Scroll area
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    // Options container
    QWidget* optionsContainer = new QWidget(this);

    QGridLayout *optionsLayout = new QGridLayout(optionsContainer);

    // Set up input grid
    for (int i=0;i<maxNumLevels;i++) {
        int baseRow=(i/2)*2;
        int baseCol=(i%2)*2;

        optionsLayout->addLayout(initLevelGUI(i), baseRow, baseCol);
    }
    optionsContainer->setLayout(optionsLayout);
    // End options Container
    scrollArea->setWidget(optionsContainer);
    // End scroll area
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(numLevelsSetter);
    mainLayout->addWidget(scrollArea);
    // end mainLayout
    setLayout(mainLayout);
}

QVBoxLayout *GameOptionsScreen::initLevelGUI(int level) {
    LevelGUIData levelGUI(this);
    guiLevels.push_back(levelGUI);

    // Layout
    QVBoxLayout *levelGUILayout = new QVBoxLayout;

    levelGUI.levelDisp->setText(("Level " + std::to_string(level)).c_str());
    levelGUILayout->addWidget(levelGUI.levelDisp);

    QFormLayout *inputLayout = new QFormLayout;
    inputLayout->addRow(levelGUI.velDisp, levelGUI.velSetter);
    inputLayout->addRow(levelGUI.posDisp, levelGUI.posSetter);

    levelGUILayout->addLayout(inputLayout);

    // Setters
    levelGUI.posSetter->setMinimum(0);
    levelGUI.posSetter->setMaximum(127);
    levelGUI.posSetter->setValue((int)(initialSetting.pos[level]*128));

    levelGUI.velSetter->setMinimum(-15);
    levelGUI.velSetter->setMaximum(15);
    levelGUI.velSetter->setValue(initialSetting.vel[level]);

    return levelGUILayout;
}
