#ifndef SCREEN
#define SCREEN

#include <graphics/DynamicGraphics.hpp>
#include <graphics/DynamicGraphicsTarget.hpp>
#include <vector>

/*
Defines a generic Screen, which can set itself as the active
    screen on the window and navigate to other screens.
Each screen has access to all other screens via a static
    member.
*/
class Screen : public DynamicGraphics {
    public:
    Screen() {
        isActive=false;
        allScreens.push_back(this);
    }
    /*
    Sets the screen to be the only visible child of it's
        parent (typically of type sf::RenderWindow).
    */
    void setActive(Screen* prevScreen) {
        std::vector<Screen*> otherScreens = getScreens();
        // Disable everything else to only display this screen.
        for (auto i : otherScreens) {
            i->setVisibility(false);
            i->isActive = false;
        }
        isActive = true;
        setVisibility(true);
    }

    void navBack() {
        prevScreen->setActive(prevScreen->getPrevScreen());
    }

    void setPrevScreen(Screen* prevScreen) {
        this->prevScreen=prevScreen;
    }

    Screen* getPrevScreen() {
        return prevScreen;
    }
    
    Screen* prevScreen; // Possible previous screen

    // Return a list with the given id
    static std::vector<Screen*> getScreens(int id) {
        std::vector<Screen*> ret;
        for (auto i : allScreens) {
            if ((*i).getId()==id) ret.push_back(i);
        }
        
        return ret;
    }
    static std::vector<Screen*> getScreens() {
        std::vector<Screen*> ret;
        for (auto i : allScreens) {
            ret.push_back(i);
        }
        
        return ret;
    }
    bool getActive() {
        return isActive;
    }
    protected:
    bool isActive;
    static std::vector<Screen*> allScreens;


};

std::vector<Screen*> Screen::allScreens;

#endif // SCREEN