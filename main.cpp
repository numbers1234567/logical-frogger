#include <graphics/DynamicGraphics.hpp>
#include <graphics/DynamicGraphicsTarget.hpp>
#include <graphics/DynamicText.hpp>
#include <screens/ScreenTypes.hpp>
#include <screens/GameOptionsScreen.hpp>
#include <screens/GameScreen.hpp>
#include <screens/StartScreen.hpp>
#include <iostream>

struct FullGameContainer
{
    DynamicGraphicsTarget& window;
    StartScreen& startScreen;
    GameOptionsScreen& gameOptionsScreen;
    GameScreen& gameScreen;
    FullGameContainer(DynamicGraphicsTarget& window,
                      StartScreen& startScreen,
                      GameOptionsScreen& gameOptionsScreen,
                      GameScreen& gameScreen) 
                      : window(window), startScreen(startScreen), 
                      gameOptionsScreen(gameOptionsScreen), gameScreen(gameScreen) {}
};


int mainloop() {
    
    DynamicGraphicsTarget window(sf::VideoMode(1024, 1024), "bruh", 0);
    StartScreen startScreen(&window);
    GameOptionsScreen gameOptions(&window);
    GameScreen gameScreen(&window);

    FullGameContainer gameContainer(window, startScreen, gameOptions, gameScreen);

    gameContainer.startScreen.setActive(nullptr);

    while (!gameContainer.window.isFinished()) {
        gameContainer.window.updateWindow();
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
    mainloop();
    return 0;
}
