#ifndef FROGGER_STATE_MACHINE
#define FROGGER_STATE_MACHINE

#include <vector>

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
    virtual int getVelocityAt(int level) const; // m/s
    virtual float getPositionAt(int level) const; // m
    virtual int getPlayerLevel() const;
    virtual int getNumLevels() const;
    virtual ~FroggerState();
    /*
    Calculate the time passed between this state and the from state.
    This is useful since the amount of time passed is an important
        identifier for a state.
    Returns a -1 if it is not possible to reach this state from "from" 
        using an incremental update.
    */
    float getTimePassed(const FroggerState& from) const;
    /*
    Updates positions for each level. For each level, it's updated as
        position := position + timestep*velocity
    */
    virtual void incrementalUpdate(float timestep);
    virtual bool movePlayer(int to);
    /*
    Returns the amount of time in seconds before the player is allowed to move, or change levels.
    Returns a -1 if the player will inevitably lose from this point.
    Returns 0 if the player is currently in a collision.
    */
    float nextInterestingState() const;

    bool isEqual(const FroggerState& other) const;
    private:
    // Next time levels l1 and l2 coincide
    float nextCollision(int l1, int l2) const;

    bool getTimePassedAux(const FroggerState& from, float t) const;
};

/*
Contains the full data on a game state in a single class.
Contains full velocity, position, and player position data
    as well as convenient conversions to smaller representations.
Allows for the update:
 *
 * imestep, modifies the positions by position+=velocity*timestep.
*/
class FroggerFullState : public FroggerState {
    public:
    FroggerFullState();
    /*Construct state directly using the obviously-required data*/
    FroggerFullState(std::vector<int> velocities, std::vector<float> positions, int playerLevel);

    /*Construct from an existing state*/
    FroggerFullState(FroggerState& state);

    ~FroggerFullState();

    int getVelocityAt(int level) const;
    float getPositionAt(int level) const;
    int getPlayerLevel() const;
    int getNumLevels() const;

    void incrementalUpdate(float timestep);
    bool movePlayer(int to);

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
    FroggerPartialState(FroggerState& initialState, float timeElapsed, int playerLevel);

    /*Constructor from given full state*/
    FroggerPartialState(FroggerState& initialState, FroggerState& toState);

    ~FroggerPartialState();

    const FroggerState& getInitialState() const;
    
    int getVelocityAt(int level) const;
    int getPlayerLevel() const;
    float getPositionAt(int level) const;
    int getNumLevels() const;
    void incrementalUpdate(float timestep);
    bool movePlayer(int to);

    private:
    FroggerState& initialState;
    float timePassed;
    int playerLevel;
};

#endif // FROGGER_STATE_MACHINE
