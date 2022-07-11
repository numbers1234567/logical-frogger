#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include <state-machine/FroggerStateMachine.hpp>

#include <GameSettings.h>

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>

#include <vector>

/* The GameScreen class */

class GameScreen : public QWidget {
public:
    GameScreen(GameSetting initialSetting);


private:
    GameSetting initialSetting;
};

#endif // GAMESCREEN_H
