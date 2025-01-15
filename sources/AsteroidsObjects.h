#pragma once

#include "GameList.h"
#include "GameObjects.h"
#include "Asteroid.h"
#include "Bullet.h"
#include "Starship.h"
#include "PowerUp.h"
#include "GameClock.h"


#define MAX_OBJECTS 500


class Asteroids : public ObjectList<Asteroid>
{
protected:
	Time        lastSpawn;
  float       spawnPeriod;

  float       minSpeed;
  float       maxSpeed;
  float       minRadius;
  float       maxRadius;
  float       minRadiusToHaveCraters;
  float       minCraterRadius;
  float       maxCraterRadius;
  int         minFragments;
  int         maxFragments;

  float       minSpawnTime;
  float       minSpawnTimeRadiusBased;
  float       maxSpawnTime;
  float       timeBeforeMaxSize;
  float       timeBeforeMaxSpawnFreq;

public:
	Asteroids (void);

public:
  
  Time		    getLastSpawn          (void)          { return lastSpawn; }
  void        setLastSpawn          (void)          { lastSpawn = GameClock::clock().getCurrentUpdate(); }
  float		    getSpawnPeriod        (void)          { return spawnPeriod; }
  void        setSpawnPeriod        (float period)  { spawnPeriod = period; }

  float       getMaxSpeed           (void)          { return maxSpeed; }
  float       getMinRadius          (void)          { return minRadius; }
  float       getMaxRadius          (void)          { return maxRadius; }
  float       getMinRadiusToHaveCraters (void)      { return minRadiusToHaveCraters; }
  float       getMinCraterRadius    (void)          { return minCraterRadius; }
  float       getMaxCraterRadius    (void)          { return maxCraterRadius; }
  int         getMinFragments       (void)          { return minFragments; }
  int         getMaxFragments       (void)          { return maxFragments; }

protected:
  void        clean     (void);

public:
  void        spawn     (int performances);
  void        update    (int performances);
  void        draw      (int performances);
  void        reset     (void);
};


class Bullets : public ObjectList<Bullet>
{
public:
	Bullets   (void);
};


class PowerUps : public ObjectList<PowerUp>
{
protected:
  Time   lastSpawn;
  float  spawnTime;

public:
	PowerUps  (void);

protected:
  void spawn  (int performances);

public:
  void update (int performances);
  void reset  (void);
};

class Starships: public ObjectList<Starship>
{
protected:
  

public:
  Starships    (void);

public:
  void update  (int performances);
  void draw    (int performances);
};


class AsteroidsObjects : public ObjectList<RenderedObject>
{
protected:
  Asteroids    asteroids;
  Bullets      bullets;
  PowerUps     powerUps;
  Starships    starships;

public:
  static AsteroidsObjects *asteroidsObjects;

	static AsteroidsObjects &objects    (void);
  AsteroidsObjects  (void);

public:
  Asteroids     &getAsteroids     (void)      { return asteroids; }
  Bullets       &getBullets       (void)      { return bullets; }
  PowerUps      &getPowerUps      (void)      { return powerUps; }
  Starships     &getStarships     (void)      { return starships; }

public:
  void update (int performances);
  void draw   (int performances);
  void reset  (void);
};
