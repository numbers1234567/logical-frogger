#ifndef START_SCREEN
#define START_SCREEN

#include <graphics/DynamicGraphics.hpp>
#include <graphics/DynamicText.hpp>
#include <graphics/DynamicButton.hpp>
#include <screens/Screen.hpp>
#include <iostream>

class StartScreenButton : public DynamicButton {
    public:
    StartScreenButton(Screen* startScreen, Screen *toScreen, int id=0) {
        this->startScreen = startScreen;
        this->toScreen = toScreen;
        setVisibility(true);

        startScreen->addChild(this);
        setId(id);
    }
    bool pointCollides(float x, float y) {
        float offset[2];
        float scale[2];
        getGlobalOffsets(offset, scale);
        return (x >= offset[0] && x <= offset[0] + scale[0]) &&
               (y >= offset[1] && y <= offset[1] + scale[1]);
    }
    void openOtherScreen() {
        toScreen->setActive();
        toScreen->prevScreen = startScreen;
    }
    void updateThis(sf::RenderWindow& target, float offset[2], float scale[2]) {
        int windowX=target.getSize().x;
        int windowY=target.getSize().y;
        sf::RectangleShape toDraw = sf::RectangleShape(sf::Vector2f(scale[0]*windowX, scale[1]*windowY));
        toDraw.setPosition(sf::Vector2f(offset[0]*windowX, offset[1]*windowY));
        toDraw.setFillColor(sf::Color(120, 250, 120));

        target.draw(toDraw);
    }
    void onHover(float x, float y) {
        std::cout << x << " " << y << std::endl;
    }
    protected:
    Screen* startScreen;
    Screen* toScreen;
};

class StartScreen : public Screen {
    public:
    StartScreen(DynamicGraphicsTarget* parent, int id=0) {
        parent->addChild(this);
        setId(id);

        initTitle();
        initButtons();
        setVisibility(true);

    }

    ~StartScreen() {
        delete titleText;
        delete start;
    }

    protected:
    void initTitle() {
        titleText = new DynamicText(this);
        titleText->setId(titleTextID);
        float offset[2] = {0.425, 0};
        titleText->setOffset(offset);
    }

    void initButtons() {
        start = new StartScreenButton(this, nullptr, startButtonID);
        float offset[2] = {0.2, 0.4};
        float scale[2] = {0.2, 0.075};
        start->setOffset(offset);
        start->setScale(scale);
    }

    const static int titleTextID=1;
    const static int startButtonID=2;
    DynamicText* titleText;
    StartScreenButton* start;
};

#endif // START_SCREEN