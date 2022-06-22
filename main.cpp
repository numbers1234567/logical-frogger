#include <graphics/DynamicGraphics.hpp>
#include <graphics/DynamicGraphicsTarget.hpp>
#include <graphics/DynamicText.hpp>
#include <screens/StartScreen.hpp>
#include <iostream>

int mainloop() {
    DynamicGraphicsTarget* window = new DynamicGraphicsTarget(sf::VideoMode(1024, 1024), "bruh", 0);
    StartScreen screen(window, 1);
    screen.setActive();
    while (!window->isFinished()) {
        window->updateWindow();
    }

    delete window;
    return 0;
}

bool init() {
    bool success=true;
    success = success&&loadAllFonts();
    return success;
}

int main(int argc, char const *argv[])
{
    init();
    mainloop();
    return 0;
}
