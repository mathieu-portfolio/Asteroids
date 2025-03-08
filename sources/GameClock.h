#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

using namespace sf;


/**
 * @class GameClock
 * @brief Manages game timing and updates.
 */
class GameClock: public Clock
{
protected:
    Time        currentUpdate;  ///< Time of the current update.
    Time        deltaUpdate;    ///< Time difference between updates.
    Time        currentRender;  ///< Time of the last render.
    Time        startingTime;   ///< Time when the game started.
    
    static GameClock *gameClock; ///< Singleton instance of GameClock.

    /**
     * @brief Constructs the GameClock.
     */
    GameClock(void);
    
    /**
     * @brief Destroys the GameClock.
     */
    ~GameClock(void);

public:
    /**
     * @brief Gets the singleton instance of GameClock.
     * @return Reference to the GameClock instance.
     */
    static GameClock &clock(void);

public:
    Time		getCurrentUpdate  (void)          { return currentUpdate; }
    void        setCurrentUpdate  (void);
    Time		getDeltaUpdate    (void)          { return deltaUpdate; }
    Time		getCurrentRender  (void)          { return currentRender; }
    void        setCurrentRender  (void)          { currentRender = getElapsedTime(); }
    Time		getStartingTime   (void)          { return startingTime; }
    void        setStartingTime   (void)          { startingTime = getElapsedTime(); }

public:
    /**
     * @brief Resets the clock.
     */
    void reset(void);
};

/**
 * @class PeriodicClock
 * @brief A clock that tracks periodic events.
 */
class PeriodicClock: public Clock
{
protected:
    Time period; ///< Period of the clock.
    Time timeOffset; ///< Offset from the start.

public:
    /**
     * @brief Constructs a PeriodicClock with a period and offset.
     * @param period The period of the clock.
     * @param offset The offset time.
     */
    PeriodicClock(const Time &period, const Time &offset);

    /**
     * @brief Constructs a PeriodicClock with a period.
     * @param period The period of the clock.
     */
    PeriodicClock(const Time &period = Time::Zero);

public:
    /**
     * @brief Gets a value based on whether the clock is "on".
     * @param onValue The value when the clock is active.
     * @param offValue The value when the clock is inactive.
     * @return Either onValue or offValue based on clock state.
     */
    template<typename T>
    T getValue(const T &onValue, const T &offValue) { return isOn() ? onValue : offValue; }

    void  reset      (const Time &offset = Time::Zero);
    void  setPeriod  (const Time &newPeriod)   { period = newPeriod; }

    Time  getPeriodicTime (void);
    Time  getPeriod       (void) { return period; }
    bool  isOn            (void);
};
