#include "state-machine/FroggerStateMachine.hpp"
#include <stdexcept>
#include <cmath>
#include <algorithm>

const float rounding_error=0.000001;
/*
 *  *  *  *  *  *
 *  BASE CLASS  *
 *  *  *  *  *  *
 */

int FroggerState::getVelocityAt(int level) const {
    std::runtime_error("FroggerState::getVelocityAt() is not overridden.");
}
float FroggerState::getPositionAt(int level) const {
    std::runtime_error("FroggerState::getPositionAt() is not overriddent");
}
int FroggerState::getPlayerLevel() const {
    std::runtime_error("FroggerState::getPlayerLevel() is not overriddent");
}
float FroggerState::getTimePassed(const FroggerState& from) const {
    // t=(k+r-r0)/v, where 0<=t<1 and k is an integer
    int kStart = -std::floor(getPositionAt(0)-from.getPositionAt(0));
    int kEnd = std::ceil(getVelocityAt(0) - getPositionAt(0) + from.getPositionAt(0));
    for (int k=kStart;k<=kEnd;k++) {
        float t=(k+getPositionAt(0)-from.getPositionAt(0))/getVelocityAt(0);
        if (getTimePassedAux(from, t)) return t;
    }
    return -1;
}
int FroggerState::getNumLevels() const {
    std::runtime_error("FroggerState::getNumLevels() is not overriddent");
}

FroggerState::~FroggerState() {}

void FroggerState::incrementalUpdate(float timestep) {
    std::runtime_error("FroggerState::incrementalUpdate() is not overridden");
}
bool FroggerState::movePlayer(int to) {
    std::runtime_error("FroggerState::movePlayer() is not overridden");
}

float FroggerState::nextInterestingState() const {
    float downTime=std::numeric_limits<float>::max();
    float upTime=std::numeric_limits<float>::max();
    bool lose=true;
    int playerLevel = getPlayerLevel();
    if (playerLevel > 0) {
        float nextTime = nextCollision(playerLevel, playerLevel-1);
        float nextPosition = getPositionAt(playerLevel) + getVelocityAt(playerLevel)*nextTime;
        if (std::abs(nextPosition-0.5) <= 0.5) {
            downTime = nextTime;
            lose=false;
        }
    }
    if (playerLevel < getNumLevels()-1) {
        float nextTime = nextCollision(playerLevel, playerLevel+1);
        float nextPosition = getPositionAt(playerLevel) + getVelocityAt(playerLevel)*nextTime;
        if (std::abs(nextPosition-0.5) <= 0.5) {
            upTime = nextTime;
            lose=false;
        }
    }
    if (lose) return -1;
    return std::min(downTime, upTime);
}

bool FroggerState::isEqual(const FroggerState& other) const {
    for (int i=0;i<getNumLevels();i++) {
        if (std::abs(other.getPositionAt(i)-getPositionAt(i)) >= rounding_error) return false;
        if (getVelocityAt(i) != getVelocityAt(i)) return false;
    }
    return true;
}

float FroggerState::nextCollision(int l1, int l2) const {
    // pPos + pVel*t == oPos + oVel*t (mod 1)
    // t(pVel-oVel) == oPos-pPos + k
    float pPos = getPositionAt(l1);
    float oPos = getPositionAt(l2);
    int pVel = getVelocityAt(l1);
    int oVel = getVelocityAt(l2);
    int k=-std::floor(oPos-pPos);
    if (pVel < oVel) k=-std::ceil(oPos-pPos);
    return (oPos-pPos+k)/(pVel-oVel);
}

bool FroggerState::getTimePassedAux(const FroggerState& from, float t) const {
    // Compare approximate positions after time t passes, then compare to actual.
    for (int i=0;i<getNumLevels();i++) {
        float approxPos = from.getPositionAt(i) + t*from.getVelocityAt(i);
        approxPos -= std::floor(approxPos);
        if (std::abs(approxPos-getPositionAt(i)) > rounding_error) return false;
    }
    return true;
}

/*
 *  *  *  *  *  *
 *  FULL STATE  *
 *  *  *  *  *  *
 */

FroggerFullState::FroggerFullState(std::vector<int> velocities, std::vector<float> positions, int playerLevel) {
    this->velocities = velocities;
    this->positions = positions;
    if (velocities.size() != positions.size()) throw std::invalid_argument("Cannot instantiate FroggerFullState with velocities.size() != positions.size()");
    numLevels = velocities.size();
    this->playerLevel = playerLevel;
}

FroggerFullState::FroggerFullState() {}

FroggerFullState::~FroggerFullState() = default;

int FroggerFullState::getVelocityAt(int level) const {
    return velocities[level];
}
float FroggerFullState::getPositionAt(int level) const {
    return positions[level] - std::floor(positions[level]);
}
int FroggerFullState::getPlayerLevel() const {
    return playerLevel;
}
int FroggerFullState::getNumLevels() const {
    return numLevels;
}

void FroggerFullState::incrementalUpdate(float timestep) {
    for (int i=0;i<getNumLevels();i++) {
        positions[i] = positions[i]+(velocities[i]*timestep);
        positions[i] -= std::floor(positions[i]);
    }
}

bool FroggerFullState::movePlayer(int to) {
    if (to >= getNumLevels() || to < 0) return false;
    playerLevel = to;
    return true;
}

/*
 *  *  *  *  *  *  *
 *  PARTIAL STATE  *
 *  *  *  *  *  *  *
 */

FroggerPartialState::FroggerPartialState(FroggerState& initialState, float timeElapsed, int playerLevel) : initialState(initialState) {
    this->timePassed=timeElapsed;
    this->playerLevel=playerLevel;
}
FroggerPartialState::FroggerPartialState(FroggerState& initialState, FroggerState& toState) : initialState(initialState) {
    timePassed=toState.getTimePassed(initialState);
    playerLevel=toState.getPlayerLevel();
    this->initialState=initialState;
    // No solution t for positions0 + velocities*t == positions1 (mod 1).
    if (timePassed==-1) throw std::invalid_argument("FroggerPartialState constructor unable to solve: toState cannot be reached from initialState.");
}

FroggerPartialState::~FroggerPartialState(){}

const FroggerState& FroggerPartialState::getInitialState() const {
    return initialState;
}
int FroggerPartialState::getVelocityAt(int level) const {
    return initialState.getVelocityAt(level);
}
int FroggerPartialState::getPlayerLevel() const {
    return playerLevel;
}
float FroggerPartialState::getPositionAt(int level) const {
    float answer=getInitialState().getPositionAt(level)+getInitialState().getVelocityAt(level)*timePassed;
    return answer - std::floor(answer);
}
int FroggerPartialState::getNumLevels() const {
    return getInitialState().getNumLevels();
}

void FroggerPartialState::incrementalUpdate(float timestep) {
    timePassed += timestep;
}
bool FroggerPartialState::movePlayer(int to) {
    if (to >= getNumLevels() || to < 0) return false;
    playerLevel=to;
    return true;
}
