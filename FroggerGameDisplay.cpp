#include "FroggerGameDisplay.h"

/* * * * * * * * * * * * *
 * FROGGER GAME DISPLAY  *
 * * * * * * * * * * * * */

FroggerGameDisplay::FroggerGameDisplay(const FroggerState& primState) :
    QGraphicsScene(),
    primState(primState) {
    // Initialize graphics for each level
    for (int i=0;i<primState.getNumLevels();i++) {
        LevelDisplay disp(this, primState.getPositionAt(i), primState.getVelocityAt(i), 0, yDisplayOffset);
        graphicsObjs.push_back(disp);
    }
}

void FroggerGameDisplay::update() {
    for (auto levelGraphics : graphicsObjs) levelGraphics.update();
}

/* * * * * * * * * * * * * * * * * * *
 * FROGGERGAMEDISPLAY::LEVELDISPLAY  *
 * * * * * * * * * * * * * * * * * * */

FroggerGameDisplay::LevelDisplay::LevelDisplay(QGraphicsScene* scene, const float& statePos, const int& stateVel, float displayY, const float& yDisplayOffset) :
    stateMachinePos(statePos),
    stateMachineVel(stateVel) {
    sceneRect = scene->sceneRect();
    this->yDisplay = displayY;

    // Initialize qt graphics objects
    logMarker = scene->addEllipse(0, 0, logMarkerDiameter, logMarkerDiameter);
    posDisplay = scene->addText(std::to_string(statePos).c_str());
    velDisplay = scene->addText(std::to_string(stateVel).c_str());

    posDisplay->setPos(sceneRect.x() + sceneRect.width()*0.8, yDisplay+yDisplayOffset);
    velDisplay->setPos(sceneRect.x() + sceneRect.width()*0.8, yDisplay+yDisplayOffset);

    update();
}

void FroggerGameDisplay::LevelDisplay::update() {
    float markerXCenter=sceneRect.x() + sceneRect.width()*stateMachinePos*0.8;

    posDisplay->setPlainText(std::to_string(stateMachinePos).c_str());
    velDisplay->setPlainText(std::to_string(stateMachineVel).c_str());
    logMarker->setPos(markerXCenter - logMarkerDiameter/2, yDisplay + yDisplayOffset - logMarkerDiameter/2);
}
