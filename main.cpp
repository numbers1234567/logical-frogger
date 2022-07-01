#include <QApplication>
#include <state-machine/FroggerStateMachine.hpp>
#include <screens/StartScreen.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    StartScreen start;
    start.show();
    return a.exec();
}
