#pragma once

#include "GameList.h"
#include "GameObjects.h"
#include "Asteroid.h"
#include "Bullet.h"
#include "Starship.h"
#include "PowerUp.h"
#include "GameClock.h"


#define MAX_OBJECTS 500 ///< Maximum number of game objects.

/**
 * @class Asteroids
 * @brief Manages a list of asteroid objects.
 */
class Asteroids : public ObjectList<Asteroid>
{
protected:
  Time  lastSpawn; ///< Time of last asteroid spawn.
  float spawnPeriod; ///< Interval between spawns.

  float minSpeed, maxSpeed; ///< Speed range of asteroids.
  float minRadius, maxRadius; ///< Size range of asteroids.
  float minRadiusToHaveCraters; ///< Minimum size to have craters.
  float minCraterRadius, maxCraterRadius; ///< Range of crater sizes.
  int minFragments, maxFragments; ///< Number of fragments after destruction.

  float minSpawnTime, minSpawnTimeRadiusBased; ///< Minimum spawn time based on radius.
  float maxSpawnTime; ///< Maximum spawn time.
  float timeBeforeMaxSize; ///< Time before asteroids reach max size.
  float timeBeforeMaxSpawnFreq; ///< Time before max spawn frequency.

public:
  /**
   * @brief Constructs an Asteroids object.
   */
  Asteroids (void);

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
  void clean(void); ///< Cleans up asteroid list.

public:
  void spawn(int performances); ///< Spawns new asteroids.
  void update(int performances); ///< Updates asteroid positions.
  void draw(int performances); ///< Draws asteroids.
  void reset(void); ///< Resets asteroid list.
};


/**
 * @class Bullets
 * @brief Manages a list of bullets.
 */
class Bullets : public ObjectList<Bullet>
{
public:
  /**
   * @brief Constructs a Bullets object.
   */
	Bullets   (void);
};


/**
 * @class PowerUps
 * @brief Manages a list of power-ups.
 */
class PowerUps : public ObjectList<PowerUp>
{
protected:
  Time  lastSpawn; ///< Last spawn time for power-ups.
  float spawnTime; ///< Spawn interval.

public:
  /**
   * @brief Constructs a PowerUps object.
   */
	PowerUps  (void);

protected:
  void spawn(int performances); ///< Spawns a new power-up.

public:
  void update(int performances); ///< Updates power-up states.
  void reset(void); ///< Resets power-up list.
};

/**
 * @class Starships
 * @brief Manages a list of starships.
 */
class Starships: public ObjectList<Starship>
{
public:
  /**
   * @brief Constructs a Starships object.
   */
  Starships    (void);

  void update(int performances); ///< Updates starship positions.
  void draw(int performances); ///< Draws starships.
};


/**
 * @class AsteroidsObjects
 * @brief Central manager for all game objects.
 */
class AsteroidsObjects : public ObjectList<RenderedObject>
{
protected:
  Asteroids  asteroids; ///< List of asteroids.
  Bullets    bullets; ///< List of bullets.
  PowerUps   powerUps; ///< List of power-ups.
  Starships  starships; ///< List of starships.

public:
  static AsteroidsObjects *asteroidsObjects; ///< Static instance of game objects.

  /**
   * @brief Gets the singleton instance of AsteroidsObjects.
   * @return Reference to the AsteroidsObjects instance.
   */
  static AsteroidsObjects &objects(void);

  /**
   * @brief Constructs an AsteroidsObjects object.
   */
  AsteroidsObjects(void);

public:
  Asteroids     &getAsteroids     (void)      { return asteroids; }
  Bullets       &getBullets       (void)      { return bullets; }
  PowerUps      &getPowerUps      (void)      { return powerUps; }
  Starships     &getStarships     (void)      { return starships; }

public:
  void update(int performances); ///< Updates all game objects.
  void draw(int performances); ///< Draws all game objects.
  void reset(void); ///< Resets all game objects.
};
