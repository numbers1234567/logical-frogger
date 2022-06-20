#ifndef DYNAMIC_GRAPHICS_TARGET
#define DYNAMIC_GRAPHICS_TARGET

#include <graphics/DynamicGraphics.hpp>

class DynamicGraphicsTarget : public DynamicGraphics, public sf::RenderWindow {
    public:
    void updateThis(const DynamicGraphicsTarget& target, float offset[], float scale[]) {
        clear();
    }
    protected:
    void onCreate() {
        bboxEnabled=false;
        parent=nullptr;
    }
    void onResize() {
        // I intend to set size and aspect ratio limitations so a user can't just break things.
    }
};

#endif // DYNAMIC_GRAPHICS_TARGET