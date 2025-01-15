#include "GameClock.h"


GameClock *GameClock::gameClock = NULL;

GameClock &GameClock::clock       (void)
{
  if (gameClock == NULL)
    gameClock = new GameClock();
  return *gameClock;
}

GameClock::GameClock              (void)
{
	reset();
}

GameClock::~GameClock             (void)
{
}

void GameClock::setCurrentUpdate  (void)
{
  auto currentTime = getElapsedTime();
	deltaUpdate      = currentTime - currentUpdate;
	currentUpdate    = currentTime;
}

void GameClock::reset             (void)
{
  setCurrentUpdate();
  setCurrentRender();
  setStartingTime();
}


PeriodicClock::PeriodicClock(
  const Time &period,
  const Time &offset
) :
  period        (period),
  timeOffset    (offset)
{
}

PeriodicClock::PeriodicClock(
  const Time &period
) :
  PeriodicClock(period, Time::Zero)
{
}

void PeriodicClock::reset(const Time &offset)
{
  restart();
  timeOffset = offset;
}

Time PeriodicClock::getPeriodicTime (void)
{
  auto milliTime    = (getElapsedTime() - timeOffset).asMilliseconds();
  auto milliPeriod  = period.asMilliseconds();
  auto periodicTime = milliTime % milliPeriod;
  return milliseconds(periodicTime);
}

bool PeriodicClock::isOn(void)
{
  auto milliTime    = getPeriodicTime().asMilliseconds();
  auto milliPeriod  = period.asMilliseconds();
  return milliTime < milliPeriod / 2;
}
