#ifndef DYNAMIC_BUTTON
#define DYNAMIC_BUTTON

#include <graphics/DynamicGraphics.hpp>
#include <vector>

/*
DynamicGraphics which tracks mouse events.
*/
class DynamicButton : public DynamicGraphics {
    public:
    /*
    Determine whether the point collides with the button. 
    */
    virtual bool pointCollides(float x, float y) {};

    void handleEvents(sf::RenderWindow& target) {
        // Collision detection
        const std::vector<sf::Event>& events = getEvents();
        int windowX = target.getSize().x;
        int windowY = target.getSize().y;
        for (auto i : events) {
            // Normalize scale to 1.
            float normalizedX;
            float normalizedY;
            switch (i.type) {
            case sf::Event::MouseButtonPressed:
                normalizedX = i.mouseButton.x*1./windowX;
                normalizedY = i.mouseButton.y*1./windowY;
                if (i.mouseButton.button != sf::Mouse::Left) break;
                if (pointCollides(normalizedX, normalizedY))
                    onClick(normalizedX, normalizedY);
                break;
            case sf::Event::MouseMoved:
                normalizedX = i.mouseMove.x*1./windowX;
                normalizedY = i.mouseMove.y*1./windowY;
                if (pointCollides(normalizedX, normalizedY))
                    onHover(normalizedX, normalizedY);
                else
                    onReleaseHover(normalizedX, normalizedY);
                break;
            default:
                break;
            }
        }
    }

    /*
    x and y range from 0 to 1.
    */
    virtual void onHover(float x, float y) {};
    virtual void onReleaseHover(float x, float y) {};
    virtual void onClick(float x, float y) {};
    virtual void onRelease(float x, float y) {};
};


#endif // DYNAMIC_BUTTON