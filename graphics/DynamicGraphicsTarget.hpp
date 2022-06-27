#ifndef DYNAMIC_GRAPHICS_TARGET
#define DYNAMIC_GRAPHICS_TARGET

#include <graphics/DynamicGraphics.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
class DynamicGraphicsTarget : public DynamicGraphics, public sf::RenderWindow {
    public:
    DynamicGraphicsTarget(sf::VideoMode vm, const sf::String &title, int id) : sf::RenderWindow(vm, title) {
        parent=nullptr;
        setId(id);
        finished = false;
    }
    void updateThis(sf::RenderWindow& target, float offset[], float scale[], float dt) {
        clear(sf::Color::Black);
    }

    bool setFinished() { 
        finished=true; 
        close();
    }
    bool isFinished() { 
        return finished; 
    }

    void updateWindow() {
        sf::Time elapsed=clock.restart();
        pollAllEvents();
        float offset[2];
        float scale[2];
        getGlobalOffsets(offset, scale);
        
        this->update(*this, offset, scale, elapsed.asSeconds());

        display();
    }

    /*
    Poll all events from the window, which can then be retrieved
        via getEvents().
    */
    void pollAllEvents() {
        sf::Event event;
        std::vector<sf::Event> empty;
        std::swap(events, empty);
        while (pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                setFinished();
            case sf::Event::Resized:
            default:
                events.push_back(event);
                break;
            }
        }
    }

    protected:
    void onCreate() {
    }
    void onResize() {
        // I intend to set size and aspect ratio limitations so a user can't just break things.
    }
    bool finished;
    sf::Clock clock;
};
#endif