#ifndef DYNAMIC_GRAPHICS_TARGET
#define DYNAMIC_GRAPHICS_TARGET

#include <graphics/DynamicGraphics.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
class DynamicGraphicsTarget : public DynamicGraphics, public sf::RenderWindow {
    public:
    DynamicGraphicsTarget(sf::VideoMode vm, const sf::String &title, int id) : sf::RenderWindow(vm, title) {
        bboxEnabled=false;
        parent=nullptr;
        setVisibility(true);
        float offset[2];
        float scale[2];
        offset[0]=0;
        offset[1]=0;
        scale[0]=1;
        scale[1]=1;
        setOffset(offset);
        setScale(scale);
    }
    void updateThis(const sf::RenderWindow& target, float offset[], float scale[]) {
        clear(sf::Color::Black);
    }

    void updateWindow() {
        float offset[2];
        float scale[2];
        getGlobalOffsets(offset, scale);
        
        this->update(*this, offset, scale);

        display();
    }

    /*
    Poll all events from the window, which can then be retrieved
        via getEvents().
    */
    void pollAllEvents() {
        sf::Event event;
        std::queue<sf::Event> empty;
        std::swap(events, empty);
        while (pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                close();
            case sf::Event::Resized:
            default:
                events.push(event);
                break;
            }
        }
    }

    std::queue<sf::Event> getEvents() { return events; }

    protected:
    void onCreate() {
    }
    void onResize() {
        // I intend to set size and aspect ratio limitations so a user can't just break things.
    }
    std::queue<sf::Event> events;
};
#endif