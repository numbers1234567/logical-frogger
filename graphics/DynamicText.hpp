#ifndef DYNAMIC_TEXT
#define DYNAMIC_TEXT

#include <graphics/DynamicGraphics.hpp>
#include <iostream>

sf::Font arial;

bool loadAllFonts() {
    bool success=true;
    success = success&&arial.loadFromFile("./graphics/fonts/arial.ttf");

    return success;
}

class DynamicText : public DynamicGraphics, public sf::Text {
    public:
    DynamicText(DynamicGraphics* parent, sf::String text="bruh moment", int charSize=24, sf::Color fillColor=sf::Color::White, sf::Font=arial, int textStyle=sf::Text::Regular, int id=0) {
        parent->addChild(this);
        setId(id);

        setFont(arial);
        setString(text);
        setCharacterSize(charSize);
        setFillColor(fillColor);
        setStyle(textStyle);
    }
    void updateThis(sf::RenderWindow& target, float offset[], float scale[]) {
        sf::Vector2f position(offset[0]*target.getSize().x, offset[1]*target.getSize().y);
        
        setPosition(position);
        target.draw(*this);
    }
};

#endif // DYNAMIC_TEXT