#ifndef GAME_SCREEN
#define GAME_SCREEN

#include <screens/Screen.hpp>
#include <screens/ScreenTypes.hpp>
#include <state-machine/FroggerStateMachine.hpp>

const int FroggerGraphicsID = 1;

class FroggerGraphics : public DynamicGraphics {
    public:
    FroggerGraphics(const FroggerState& state, float timeC=0.1, int id=FroggerGraphicsID) {
        initState(state);
        setTimeC(timeC);
        setId(id);
    }
    float getTimeC() { return timeC; }
    const FroggerState& getState() { return state; }
    void setTimeC(float timeC) { this->timeC=timeC; }
    /*
    Initialize this state from the given state
    */
    void initState(const FroggerState& state) {
        int numLevels=state.getNumLevels();
        std::vector<float> pos;
        std::vector<int> vel;
        for (int i=0;i<numLevels;i++) {
            pos.push_back(state.getPositionAt(i));
            vel.push_back(state.getVelocityAt(i));
        }
        this->state = FroggerFullState(vel, pos, 0);
    }

    void drawLevel(sf::RenderWindow& target, float offset[], float scale[], int level) {
        const FroggerState& state = getState();
        int numLevels = state.getNumLevels();
        float yPos = offset[1] + scale[1] - scale[1]/(2*numLevels);
        yPos -= level * scale[1] / numLevels;
        float xPos = offset[0] + 0.8 * scale[1] * state.getPositionAt(level);
        sf::CircleShape circle(2);
        circle.setOutlineColor(sf::Color(200, 200, 200));
        circle.setPosition(sf::Vector2f(xPos, yPos));
        target.draw(circle);
    }
    void updateThis(sf::RenderWindow& target, float offset[], float scale[], float dt) {
        state.incrementalUpdate(dt*timeC);
        for (int i=0;i<state.getNumLevels();i++) 
            drawLevel(target, offset, scale, i);
    }


    protected:
    FroggerFullState state;
    float timeC;
};

class GameScreen : public Screen {
    public:
    enum currentMode {
        singlePlayer,
        solverPlayerSimul, // Solver game occuring to the right simulatenously to the player
        solverCompare, // Compare existing play
        solveOnly // Only solver
    };

    GameScreen(DynamicGraphics* parent, int id=ScreenType::game) {
        setId(id);
        parent->addChild(this);
    }

    void initSinglePlayer(FroggerState& initial) {
        int playerPos=0;
    }

    void initSolverPlayerSimul(FroggerState& initial) {

    }

    void initSolverCompare() {

    }
    void initSolveOnly(FroggerState& initial) {

    }
    
};

#endif // GAME_SCREEN