#ifndef GAME_OPTIONS_SCREEN
#define GAME_OPTIONS_SCREEN

#include <screens/Screen.hpp>
#include <state-machine/FroggerStateMachine.hpp>
#include <screens/ScreenTypes.hpp>
#include <cstdlib>
#include <ctime>

class GameOptionsScreen : public Screen {
    public:
    GameOptionsScreen(DynamicGraphics* parent, int id=ScreenType::gameOptions) {
        // Defaults
        initRandom();
        setId(id);
        parent->addChild(this);
    }
    void initRandom(int numLevels=20) {
        std::srand(std::time(nullptr));
        // velocities, positions
        std::vector<int> vel;
        std::vector<float> pos;
        for (int i=0;i<numLevels;i++) {
            vel.push_back(std::rand()%46+5);
            pos.push_back(std::rand()*1./RAND_MAX);
        }
        gameState = new FroggerFullState(vel, pos, 0);
    }
    
    void startGame() {
        getScreens(ScreenType::game)[0];
    }

    ~GameOptionsScreen() {
        delete gameState;
    }

    void updateThis(sf::RenderWindow& target, float offset[], float scale[]) {
        sf::RectangleShape rect(sf::Vector2f(200, 200));
        rect.setFillColor(sf::Color(200, 200, 200));
        target.draw(rect);
        if (getActive()) 
            getScreens(ScreenType::game)[0]->setActive(this);
        
    }
    protected:
    FroggerFullState* gameState;
};

#endif // GAME_OPTIONS_SCREEN