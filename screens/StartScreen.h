#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include <QPushButton>
#include <QHBoxLayout>
#include <screens/GameOptionsScreen.h>

/*
 Container for the start screen
*/
class StartScreen : public QWidget {
    Q_OBJECT
public:
    const static std::string screenName;

    explicit StartScreen(QWidget *parent=0);
    ~StartScreen();

    /* Make the screen visible or invisible */
    void setInactive();
    void setActive();
    bool isActive;

public slots:
    void onStartButtonPressed();
signals:
    void startButtonPressed();
    void helpButtonPressed();
private:
    /* GUI elements */
    QPushButton *startButton;
    QPushButton *helpButton;
};

#endif // STARTSCREEN_H
