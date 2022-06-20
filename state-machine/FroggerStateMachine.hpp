#ifndef FROGGER_STATE_MACHINE
#define FROGGER_STATE_MACHINE

#include <vector>
#include <unordered_set>
#include <cmath>
#include <limits>
#include <iostream>

const float rounding_error=0.000001;

/*
"Abstract class" with getters on the full data needed to define a frogger game.
Assumes that the velocity is in m/s, and that the game is 1 m wide.
Assumes that no two neighbor levels have the same velocity.
*/
class FroggerState {
    public:
    /*
    Basic getters
    */
    virtual int getVelocityAt(int level) = 0; // m/s
    virtual float getPositionAt(int level) = 0; // m
    virtual int getPlayerLevel() = 0;
    virtual int getNumLevels() = 0;
    virtual ~FroggerState() {};
    /*
    Calculate the time passed between this state and the from state.
    This is useful since the amount of time passed is an important
        identifier for a state.
    Returns a -1 if it is not possible to reach this state from "from" 
        using an incremental update.
    */
    float getTimePassed(FroggerState& from) {
        // t=(k+r-r0)/v, where 0<=t<1 and k is an integer
        int kStart = -std::floor(getPositionAt(0)-from.getPositionAt(0));
        int kEnd = std::ceil(getVelocityAt(0) - getPositionAt(0) + from.getPositionAt(0));
        for (int k=kStart;k<=kEnd;k++) {
            float t=(k+getPositionAt(0)-from.getPositionAt(0))/getVelocityAt(0);
            if (getTimePassedAux(from, t)) return t;
        }
        return -1;
    }
    /*
    Updates positions for each level. For each level, it's updated as
        position := position + timestep*velocity
    */
    virtual void incrementalUpdate(float timestep) = 0;
    virtual bool movePlayer(int to) = 0;
    /*
    Returns the amount of time in seconds before the player is allowed to move, or change levels.
    Returns a -1 if the player will inevitably lose from this point.
    Returns 0 if the player is currently in a collision.
    */
    float nextInterestingState() {
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
    private:
    // Next time levels l1 and l2 coincide
    float nextCollision(int l1, int l2) {
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

    bool getTimePassedAux(FroggerState& from, float t) {
        // Compare approximate positions after time t passes, then compare to actual.
        for (int i=0;i<getNumLevels();i++) {
            float approxPos = from.getPositionAt(i) + t*from.getVelocityAt(i);
            approxPos -= std::floor(approxPos);
            if (std::abs(approxPos-getPositionAt(i)) > rounding_error) return false;
        }
        return true;
    }
};

/*
Contains the full data on a game state in a single class.
Contains full velocity, position, and player position data
    as well as convenient conversions to smaller representations.
Allows for the update:
 * incremental update - given the timestep, modifies the positions by position+=velocity*timestep.
*/
class FroggerFullState : public FroggerState {
    public:

    /*Construct state directly using the obviously-required data*/
    FroggerFullState(std::vector<int> velocities, std::vector<float> positions, int playerLevel) {
        this->velocities = velocities;
        this->positions = positions;
        if (velocities.size() != positions.size()) throw std::invalid_argument("Cannot instantiate FroggerFullState with velocities.size() != positions.size()");
        numLevels = velocities.size();
        this->playerLevel = playerLevel;
    }

    /*Construct from an existing state*/
    FroggerFullState(FroggerState& state) {
        numLevels = state.getNumLevels();
        playerLevel = state.getPlayerLevel();

        for (int i=0;i<numLevels;i++) {
            velocities.push_back(state.getVelocityAt(i));
            positions.push_back(state.getPositionAt(i));
        }
    }

    ~FroggerFullState() = default;

    int getVelocityAt(int level) {
        return velocities[level];
    }
    float getPositionAt(int level) {
        return positions[level] - std::floor(positions[level]);
    }
    int getPlayerLevel() {
        return playerLevel;
    }
    int getNumLevels() {
        return numLevels;
    }

    void incrementalUpdate(float timestep) {
        for (int i=0;i<getNumLevels();i++) {
            positions[i] = positions[i]+(velocities[i]*timestep);
            positions[i] -= std::floor(positions[i]);
        }
    }

    bool movePlayer(int to) {
        if (to >= getNumLevels() || to < 0) return false;
        playerLevel = to;
        return true;
    }

    private:

    std::vector<int> velocities; // m/s
    std::vector<float> positions; // m
    int playerLevel; // from 0 to velocities.size()-1, which log the player is on
    int numLevels;
};

/*
This is essentially just an ID for the state, containing very bare-bones identification for better memory usage
Just needs the initial state and calculates everything from that
*/
class FroggerPartialState : public FroggerState {

    public:
    
    /*Basic constructor using the essential state elements*/
    FroggerPartialState(FroggerState& initialState, float timeElapsed, int playerLevel) : initialState(initialState) {
        this->timePassed=timeElapsed;
        this->playerLevel=playerLevel;
    }

    /*Constructor from given full state*/
    FroggerPartialState(FroggerState& initialState, FroggerState& toState) : initialState(initialState) {
        timePassed=toState.getTimePassed(initialState);
        playerLevel=toState.getPlayerLevel();
        this->initialState=initialState;
        // No solution t for positions0 + velocities*t == positions1 (mod 1).
        if (timePassed==-1) throw std::invalid_argument("FroggerPartialState constructor unable to solve: toState cannot be reached from initialState.");
    }

    ~FroggerPartialState(){}

    FroggerState& getInitialState() {
        return initialState;
    }
    
    int getVelocityAt(int level) {
        return initialState.getVelocityAt(level);
    }
    int getPlayerLevel() {
        return playerLevel;
    }
    float getPositionAt(int level) {
        float answer=getInitialState().getPositionAt(level)+initialState.getVelocityAt(level)*timePassed;
        return answer - std::floor(answer);
    }
    int getNumLevels() {
        return getInitialState().getNumLevels();
    }
    void incrementalUpdate(float timestep) {
        timePassed += timestep;
    }
    bool movePlayer(int to) {
        if (to >= getNumLevels() || to < 0) return false;
        playerLevel=to;
        return true;
    }

    private:
    FroggerState& initialState;
    float timePassed;
    int playerLevel;
};

#endif // FROGGER_STATE_MACHINE