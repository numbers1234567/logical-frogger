#include <QApplication>
#include <state-machine/FroggerStateMachine.hpp>
#include <FroggerWindow.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FroggerWindow window;
    //window.show();
    window.startScreen->setActive();
    window.show();
    return a.exec();
}
