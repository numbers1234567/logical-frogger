#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include <QPushButton>
#include <QHBoxLayout>

class StartScreen : public QWidget {
public:
    explicit StartScreen(QWidget *parent=0);
    ~StartScreen();

    QPushButton *startButton;
    QPushButton *helpButton;
};

#endif // STARTSCREEN_H
