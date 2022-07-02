#ifndef FROGGERWINDOW_H
#define FROGGERWINDOW_H

#include <QWidget>
#include <screens/StartScreen.h>
#include <screens/GameOptionsScreen.h>

class FroggerWindow : public QWidget {
public:
    explicit FroggerWindow(QWidget* parent=0);
    StartScreen* startScreen;
    GameOptionsScreen* gameOptionsScreen;
};

#endif // FROGGERWINDOW_H
