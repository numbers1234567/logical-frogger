#include <graphics/DynamicGraphics.hpp>
#include <graphics/DynamicGraphicsTarget.hpp>
#include <graphics/DynamicText.hpp>
#include <screens/ScreenTypes.hpp>
#include <screens/GameOptionsScreen.hpp>
#include <screens/GameScreen.hpp>
#include <screens/StartScreen.hpp>
#include <iostream>

struct FullDataContainer
{
    DynamicGraphicsTarget& window;
    StartScreen& startScreen;
    GameOptionsScreen& gameOptionsScreen;
    GameScreen& gameScreen;
    FullDataContainer(DynamicGraphicsTarget& window,
                      StartScreen& startScreen,
                      GameOptionsScreen& gameOptionsScreen,
                      GameScreen& gameScreen) 
                      : window(window), startScreen(startScreen), 
                      gameOptionsScreen(gameOptionsScreen), gameScreen(gameScreen) {}
};

int loop(FullDataContainer gameContainer) {
    gameContainer.window.updateWindow();
    return 0;
}

int mainloop() {
    DynamicGraphicsTarget window(sf::VideoMode(1024, 1024), "bruh", 0);
    StartScreen startScreen(&window);
    GameOptionsScreen gameOptions(&window);
    GameScreen gameScreen(&window);

    FullDataContainer gameContainer(window, startScreen, gameOptions, gameScreen);

    gameContainer.startScreen.setActive(nullptr);

    while (!gameContainer.window.isFinished()) {
        int ret = loop(gameContainer);
        if (ret!=0) return ret;
    }

    return 0;
}


bool initResources() {
    bool success=true;
    success = success&&loadAllFonts();
    return success;
}

int main(int argc, char const *argv[])
{
    initResources();
    return mainloop();
}
