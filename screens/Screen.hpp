#ifndef SCREEN
#define SCREEN

#include <graphics/DynamicGraphics.hpp>
#include <vector>

class Screen : public DynamicGraphics {
    public:
    /*
    Sets the screen to be the only visible child of it's
        parent (typically of type sf::RenderWindow).
    */
    void setActive() {
        std::vector<DynamicGraphics&> otherScreens = parent->getChildren();
        // Disable everything else to only display this screen.
        for (auto i : otherScreens) i.setVisibility(false);
        setVisibility(true);
    }
};

#endif