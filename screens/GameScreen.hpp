#ifndef GAME_SCREEN
#define GAME_SCREEN

#include <screens/Screen.hpp>
#include <screens/ScreenTypes.hpp>
class GameScreen : public Screen {
    public:
    GameScreen(DynamicGraphics* parent, int id=ScreenType::game) {
        setId(id);
        parent->addChild(this);
    }
    void setupGame(FroggerState& gameState) {
        
    }
};

#endif // GAME_SCREEN