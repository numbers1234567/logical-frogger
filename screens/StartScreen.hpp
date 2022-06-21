#ifndef START_SCREEN
#define START_SCREEN

#include <graphics/DynamicGraphics.hpp>
#include <graphics/DynamicText.hpp>
#include <screens/Screen.hpp>
#include <iostream>

class StartScreen : public Screen {
    public:
    StartScreen(DynamicGraphicsTarget* parent, int id=0) {
        parent->addChild(this);
        setId(id);

        titleText = new DynamicText(this);
        titleText->setId(titleTextID);
        float offset[2] = {0.3, 0};
        titleText->setOffset(offset);
    }
    void updateThis(sf::RenderWindow& target, float offset[], float scale[]) {
        /*sf::CircleShape shape(50.f);

        shape.setFillColor(sf::Color(150, 50, 250));
        target.draw(shape);*/
    }

    ~StartScreen() {
        delete titleText;
    }

    protected:
    const static int titleTextID=10;
    DynamicText* titleText;
};

#endif // START_SCREEN