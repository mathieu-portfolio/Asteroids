#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

using namespace sf;


class GameClock: public Clock
{
protected:
    Time        currentUpdate;
    Time        deltaUpdate;
    Time        currentRender;
    Time        startingTime;
    static  GameClock  *gameClock;
    GameClock (void);
    ~GameClock (void);
public:
    static  GameClock  &clock  	          (void);

public:
    Time		    getCurrentUpdate  (void)          { return currentUpdate; }
    void        setCurrentUpdate  (void);
    Time		    getDeltaUpdate    (void)          { return deltaUpdate; }
    Time		    getCurrentRender  (void)          { return currentRender; }
    void        setCurrentRender  (void)          { currentRender = getElapsedTime(); }
    Time		    getStartingTime   (void)          { return startingTime; }
    void        setStartingTime   (void)          { startingTime = getElapsedTime(); }

public:
    void        reset           (void);
};

class PeriodicClock: public Clock
{
protected:
    Time  period;
    Time  timeOffset;

public:
    PeriodicClock(
        const Time &period,
        const Time &offset
    );
    PeriodicClock(
        const Time &period = Time::Zero
    );

public:
    template<typename T>
    T getValue(
        const T &onValue,
        const T &offValue
    )
    {
        if(isOn())
            return onValue;
        else
            return offValue;
    }

    void  reset      (const Time &offset = Time::Zero);
    void  setPeriod  (const Time &newPeriod)   { period = newPeriod; }

    Time  getPeriodicTime (void);
    Time  getPeriod       (void) { return period; }
    bool  isOn            (void);
};
