#ifndef DYNAMIC_GRAPHICS
#define DYNAMIC_GRAPHICS

#include <list>
#include <vector>
#include <queue>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <iostream>


/*
An interface for drawing different types of things onto the screen.
The main method to know is drawThis(), which is implemented to draw
    different kinds of things.

The intended use of drawThis() is that attributes of this class
    are updated by the user. The user will define some sort of
    logic to draw onto a target window based on these dynamic
    attributes.

When updating graphics, the parent object is always displayed first. 
Children objects are displayed in the order they were added.
*/
class DynamicGraphics {
    public:
    DynamicGraphics() {
        setVisibility(true);
        float offset[2] = {0,0};
        float scale[2] = {1,1};
        setOffset(offset);
        setScale(scale);
        bboxEnabled = false;
    }
    /*
    Draw the given object using the given offset and scale value.
    For scale={1, 1} & offset={0,0}, the object should draw to the entire screen.
    Is called on each frame.
    */
    virtual void updateThis(sf::RenderWindow& target, float offset[], float scale[], float dt) {};
    /*
    Handle events given from getEvents(). 
    Is automatically called in each frame prior to drawing.
    */
    virtual void handleEvents(sf::RenderWindow& target) {};
    /*
    update the given object and its children considering the parent offset and scale.
    */
    void update(sf::RenderWindow& target, float offset[], float scale[], float dt) {
        handleEvents(target);
        if (!isVisible()) return;
        float relOffset[2];
        float relScale[2];
        getRelativeOffsets(relOffset, relScale);
        float newOffset[2] = {
            offset[0] + scale[0]*relOffset[0],
            offset[1] + scale[1]*relOffset[1]
        };
        float newScale[2] = {
            scale[0] * relScale[0],
            scale[1] * relScale[1]
        };

        updateThis(target, newOffset, newScale, dt);

        std::vector<DynamicGraphics*> children = getChildren();
        for (auto child : children) 
            child->update(target, newOffset, newScale, dt);

        if (bboxEnabled) showBBox(target, newOffset, scale);
    }

    /*
    Basic getters and setters
    */
    int getId() { return id; }
    void setId(int id) { this->id=id; }
    int getNumChildren() { return children.size(); }
    void setVisibility(bool visible) { this->visible=visible; }
    bool isVisible() { return visible; }
    /*
    Fills up the targetOffset and targetScale arrays with
        the offset and scale relative to the parent offsets.
    */
    void getRelativeOffsets(float targetOffset[2], float targetScale[2]) { 
        targetOffset[0]=offset[0];
        targetOffset[1]=offset[1];
        targetScale[0]=scale[0];
        targetScale[1]=scale[1];
    }
    /*
    Fills up the targetOffset and targetScale relative to
        the root DynamicGraphics object.
    */
    void getGlobalOffsets(float targetOffset[2], float targetScale[2]) {
        if (parent==nullptr) getRelativeOffsets(targetOffset, targetScale);
        else {
            float relOffset[2];
            float relScale[2];
            float thisOffset[2];
            float thisScale[2];
            parent->getGlobalOffsets(relOffset, relScale);
            getRelativeOffsets(thisOffset, thisScale);
            targetOffset[0] = relOffset[0]+relScale[0]*thisOffset[0];
            targetOffset[1] = relOffset[1]+relScale[1]*thisOffset[1];
            targetScale[0] = thisScale[0]*relScale[0];
            targetScale[1] = thisScale[1]*relScale[1];
        }
    }
    void setOffset(float offset[2]) {
        this->offset[0]=offset[0];
        this->offset[1]=offset[1];
    }
    void setScale(float scale[2]) {
        this->scale[0]=scale[0];
        this->scale[1]=scale[1];
    }
    
    /*
    Remove all children with the given id.
    */
    int removeChildren(int id) {
        for (std::list<DynamicGraphics*>::iterator it=children.begin();it!=children.end();) {
            if ((*it)->getId()==id) 
                it = children.erase(it);
            else ++it;
        }
    }

    /*
    Add child. Automaticcaly sets the child->parent
    */
    void addChild(DynamicGraphics* child) { 
        children.push_back(child); 
        child->parent=this;
    }

    std::vector<DynamicGraphics*> getChildren(int id) {
        std::vector<DynamicGraphics*> out;
        for (std::list<DynamicGraphics*>::iterator it=children.begin();it!=children.end();++it) {
            if ((*it)->getId()==id) 
                out.push_back(*it);
        }
        return out;
    }

    std::vector<DynamicGraphics*> getChildren() {
        std::vector<DynamicGraphics*> out;
        for (std::list<DynamicGraphics*>::iterator it=children.begin();it!=children.end();++it) {
            out.push_back(*it);
        }
        return out;
    }

    // Bounding box
    bool toggleBBox() { return (bboxEnabled=!bboxEnabled); }
    
    const std::vector<sf::Event>& getEvents() {
        return events;
    }

    protected:
    bool bboxEnabled;
    DynamicGraphics* parent;
    
    static std::vector<sf::Event> events;

    private:
    bool visible;
    // Display bounding box as a rectangle
    void showBBox(sf::RenderWindow& target, float offset[], float scale[]) {

    }

    int id;
    std::list<DynamicGraphics*> children;
    float offset[2];
    float scale[2];
};

std::vector<sf::Event> DynamicGraphics::events;

#endif // DYNAMIC_GRAPHICS