#include <graphics/DynamicGraphics.hpp>

int main(int argc, char const *argv[])
{
    DynamicGraphicsTarget window(sf::VideoMode(800, 600), "bruh", 0);
    //window.setFramerateLimit(1);
    while (window.isOpen()) {
        window.pollAllEvents();
        window.updateWindow();
    }
    
    return 0;
}
