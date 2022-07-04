#ifndef GAMEOPTIONSSCREEN_H
#define GAMEOPTIONSSCREEN_H

#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <GameSettings.h>
#include <vector>

// Compiles all GUI information into a single structure.
struct LevelGUIData {
    QSlider* velSetter;
    QSlider* posSetter;
    QLabel* velDisp;
    QLabel* posDisp;
    LevelGUIData(QWidget* parent) {
        velSetter = new QSlider(parent);
        posSetter = new QSlider(parent);
        velDisp = new QLabel(parent);
        posDisp = new QLabel(parent);
        velDisp->setText("test1");
        posDisp->setText("test2");
    }
};

class GameOptionsScreen : public QWidget {
public:
    const static std::string screenName;

    // Limit number of levels.
    const static int maxNumLevels=100;
    const static int minNumLevels=10;

    explicit GameOptionsScreen(QWidget *parent=0, int numLevels=20);

    // Can set the screen to be invisible or invisible
    void setActive();
    void setInactive();
    bool isActive;

public slots:
    // On change game setting
    void changeVelocity(int level, int newVelocity);
    void changePosition(int level, float newPosition);
    void changeNumLevels(int newNumLevels);
private:
    struct GameSetting currentSetting;

    // GUI elements
    QSlider *numLevelsSlider;
    QLabel *title;
    std::vector<LevelGUIData> guiLevels;

    int numLevels;

    // Make the option invisible or visible if the number of levels is modified
    void disableLevel(int level);
    void activateLevel(int level);

    // innit
    void initGUI();
};

#endif // GAMEOPTIONSSCREEN_H
