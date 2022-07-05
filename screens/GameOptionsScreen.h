#ifndef GAMEOPTIONSSCREEN_H
#define GAMEOPTIONSSCREEN_H

#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <GameSettings.h>
#include <QVBoxLayout>
#include <vector>

// Compiles all GUI information into a single structure.
struct LevelGUIData {
    QSlider* velSetter;
    QSlider* posSetter;
    QLabel* velDisp;
    QLabel* posDisp;
    QLabel* levelDisp;
    LevelGUIData(QWidget* parent) {
        velSetter = new QSlider(Qt::Horizontal, parent);
        posSetter = new QSlider(Qt::Horizontal, parent);
        velDisp = new QLabel(parent);
        posDisp = new QLabel(parent);
        levelDisp = new QLabel(parent);

        velDisp->setText("vel");
        posDisp->setText("vel");

        velSetter->connect(velSetter, SIGNAL(valueChanged(int)), velDisp, SLOT(setNum(int)));
        posSetter->connect(posSetter, SIGNAL(valueChanged(int)), posDisp, SLOT(setNum(int)));
    }
};

class GameOptionsScreen : public QWidget {
    Q_OBJECT
public:
    const static std::string screenName;

    // Limit number of levels.
    const static int maxNumLevels=100;
    const static int minNumLevels=10;

    explicit GameOptionsScreen(QWidget *parent=0, int numLevels=50);

    // Can set the screen to be invisible or invisible
    void setActive();
    void setInactive();
    bool isActive;

public slots:
    // On change game setting
    void changeNumLevels(int newNumLevels);
private:
    struct GameSetting initialSetting;

    // GUI elements
    std::vector<LevelGUIData> guiLevels;

    int numLevels;

    // Make the option invisible or visible if the number of levels is modified
    void disableLevel(int level);
    void activateLevel(int level);

    // innit
    void initGUI();
    QVBoxLayout* initLevelGUI(int level); // initGUI auxiliary function
};

#endif // GAMEOPTIONSSCREEN_H
