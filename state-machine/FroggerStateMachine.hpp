#include <vector>
#include <unordered_set>
#include <cmath>
#include <limits>

const float rounding_error=0.0001;

/*
"Abstract class" with getters on the full data needed to define a frogger game.
Assumes that the velocity is in m/s, and that the game is 1 m wide.
*/
class FroggerState {
    public:
    /*
    Basic getters and setters
    */
    int getVelocityAt(int level); // m/s
    float getPositionAt(int level); // m
    int getPlayerLevel();
    int getNumLevels();
    /*
    Calculate the time passed between this state and the from state.
    This is useful since the amount of time passed is an important
        identifier for a state.
    Returns a -1 if it is not possible to reach this state from "from" 
        using an incremental update.
    */
    float getTimePassed(FroggerState& from);
    void incrementalUpdate(float timestep);
    bool movePlayer(int to);
    /*
    Returns the amount of time in seconds before the player is allowed to move, or change levels.
    Returns a -1 if the player will inevitably lose from this point.
    */
    float timeUntilNextInterestingState() {
        float downTime=std::numeric_limits<float>::max();
        float upTime=std::numeric_limits<float>::max();
        bool lose=true;
        if (getPlayerLevel() > 0) {
            float playerLevel = getPlayerLevel();
            float nextTime = nextCollision(playerLevel, playerLevel-1);
            float nextPosition = getPositionAt(playerLevel) + getVelocityAt(playerLevel)*nextTime;
            if (std::abs(nextPosition-0.5) <= 0.5) {
                downTime = nextTime;
                lose=false;
            }
        }
        if (getPlayerLevel() < getNumLevels()-1) {
            float playerLevel = getPlayerLevel();
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
        int k=std::ceil(getPositionAt(l2)-getPositionAt(l1));
        return (getPositionAt(l2)-getPositionAt(l1)+k)/(getVelocityAt(l1)-getVelocityAt(l2));
    }
};

/*
Contains the full data on a game state in a single class.
Contains full velocity, position, and player position data
    as well as convenient conversions to smaller representations.
Allows for the update:
 * incremental update - given the timestep, modifies the positions by position+=velocity*timestep.
Makes the assumption that the game is 1 meter wide.
*/
class FroggerFullState : FroggerState {
    public:

    /*Construct state directly using the obviously-required data*/
    FroggerFullState(std::vector<int> velocities, std::vector<float> positions, int playerLevel) {
        this->velocities = velocities;
        this->positions = positions;
        if (velocities.size() != positions.size()) throw std::invalid_argument("Cannot instantiate FroggerFullState with velocities.size() != positions.size()");
        numLevels = velocities.size();
        this->playerLevel = playerLevel;
    }

    /*Construct from a partial state - using the initial state and time passed to infer position*/
    FroggerFullState(FroggerPartialState& partialState) {
        FroggerState& initial = partialState.getInitialState();
        numLevels = initial.getNumLevels();
        playerLevel = initial.getPlayerLevel();

        for (int i=0;i<numLevels;i++) {
            velocities.push_back(initial.getVelocityAt(i));
            positions.push_back(initial.getPositionAt(i));
        }
        
        for (int i=0;i<numLevels;i++) {
            incrementalUpdate(partialState.getTimePassed(initial));
        }
    }

    int getVelocityAt(int level) {
        return velocities[level];
    }
    float getPositionAt(int level) {
        return velocities[level];
    }
    int getPlayerLevel() {
        return playerLevel;
    }
    int getNumLevels() {
        return numLevels;
    }
    float getTimePassed(FroggerState& from) {
        // t=(k+r-r0)/v, where 0<=t<1 and k is an integer
        int kStart = std::ceil(getPositionAt(0) - from.getPositionAt(0));
        int kEnd = std::floor(getVelocityAt(0) - getPositionAt(0) + from.getPositionAt(0));
        for (int k=kStart;k<=kEnd;k++) {
            float t=(k+getPositionAt(0)-from.getPositionAt(0))/getVelocityAt(0);
            if (getTimePassedAux(from, t)) return t;
        }
        return -1;
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
    bool getTimePassedAux(FroggerState& from, float t) {
        // Compare approximate positions after time t passes, then compare to actual.
        for (int i=0;i<getNumLevels();i++) {
            float approxPos = from.getPositionAt(i) + t*from.getVelocityAt(i);
            if (std::abs(approxPos-getPositionAt(i)) > rounding_error) return false;
        }
        return true;
    }

    std::vector<int> velocities; // m/s
    std::vector<float> positions; // m
    int playerLevel; // from 0 to velocities.size()-1, which log the player is on
    int numLevels;
};

/*
This is essentially just an ID for the state, containing very bare-bones identification for better memory usage
Just needs the initial state and calculates everything from that
*/
class FroggerPartialState : FroggerState {

    public:
    
    /*Basic constructor using the essential state elements*/
    FroggerPartialState(FroggerState& initialState, float timeElapsed, int playerLevel) : initialState(initialState) {
        this->initialState=initialState;
        this->timePassed=timeElapsed;
        this->playerLevel=playerLevel;
    }

    /*Constructor from given full state*/
    FroggerPartialState(FroggerState& initialState, FroggerState& toState) : initialState(initialState) {
        timePassed=toState.getTimePassed(initialState);
        playerLevel=toState.getPlayerLevel();
        this->initialState=initialState;
        // No solution t for positions0 + velocities*t == positions1.
        if (timePassed==-1) throw std::invalid_argument("FroggerPartialState constructor unable to solve: toState cannot be reached from initialState.");
    }

    FroggerState& getInitialState() {
        return initialState;
    }
    
    int getVelocityAt(int level) {
        return initialState.getVelocityAt(level);
    }
    float getTimePassed(FroggerState& from) {
        float offset=from.getTimePassed(initialState);
        if (offset==-1) return -1;
        return timePassed-from.getTimePassed(initialState);
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