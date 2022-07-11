#ifndef FROGGERGAMEDISPLAY_H
#define FROGGERGAMEDISPLAY_H

#include <state-machine/FroggerStateMachine.hpp>>

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>

/* Method to display graphics of a Frogger state, as well as
    convenient update methods.
   This is essentially tied to an object of type FroggerState,
    and when this object is updated in any way, the display
    can be updated. */
class FroggerGameDisplay : public QGraphicsScene {
public:
    explicit FroggerGameDisplay(const FroggerState& primState);

    /* Update the display based on the properties
        given in primState */
    void update();

    const FroggerState& primState;
private:
    float yDisplayOffset;
    /* Inner class to handle qt graphics for a single level */
    class LevelDisplay {
    public:
        QGraphicsEllipseItem *logMarker;
        QGraphicsTextItem *posDisplay;
        QGraphicsTextItem *velDisplay;

        // Rect identifier for the scene.
        QRectF sceneRect;
        // y offset for scrolling. Initially 0.
        const static float yDisplayOffset;
        float yDisplay;

        const static int logMarkerDiameter;

        // pos from 0 to 1. Pos and vel taken directly from primState[level]
        const float& stateMachinePos;
        const int& stateMachineVel;

        /* Initialize the display for a level given only the state machine
            position and velocity.
           Also given is the y coordinate for the display. Specifically, the
            y position of the marker*/
        LevelDisplay(QGraphicsScene* scene, const float& statePos, const int& stateVel, float displayY, const float& yDisplayOffset);
        /* Update the levelDisplay based on the new values for stateMachinePos and stateMachineVel */
        void update();
    };

    /* Contains all qt graphics level-wise */
    std::vector<LevelDisplay> graphicsObjs;
};

#endif // FROGGERGAMEDISPLAY_H
