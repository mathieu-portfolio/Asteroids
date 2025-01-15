#pragma once

#include "GameObjects.h"
#include <list>



template<class T>
class GameList: public list<T *>
{
public:
  GameList (void){};
  virtual ~GameList(void){};

public:
  bool contains  (T *elt)
  {
    for (auto *otherElt : *this)
      if (otherElt == elt)
        return true;
  
    return false;
  }
  void add		   (T *elt)
  {
    if (elt == NULL || contains(elt))
		  return;
	  list<T *>::push_back(elt);
  }
};


template<class T>
class ObjectList: public GameList<T>
{
public:
  ObjectList (void)
  {
    // reset();
  };
  virtual ~ObjectList(void){};

public:
  virtual void update     (int performances)
  {
    for (auto elt = GameList<T>::begin(); elt != GameList<T>::end(); ) {
      auto nextElt = elt;
      nextElt++;
      auto *object = (RenderedObject *) *elt;
      object->update(performances);
      // if (object->isDestroyed())
      //   delete *elt;

      elt = nextElt;
    }
  }

  virtual void draw       (int performances)
  {
    for (auto *elt : *this)
		  ((RenderedObject *) elt)->draw(performances);
  }

  virtual void reset(void){};
};


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
};

class GameObjects : public ObjectList<RenderedObject>
{
protected:
  Asteroids  asteroids;
  Bullets    bullets;
  PowerUps   powerUps;
  Starships  starships;

public:
  static GameObjects *gameObjects;

	static GameObjects &objects    (void);
  GameObjects  (void);

public:
  Asteroids &getAsteroids (void) { return asteroids; }
  Bullets   &getBullets   (void) { return bullets; }
  PowerUps  &getPowerUps  (void) { return powerUps; }
  Starships &getStarships (void) { return starships; }

public:
  void update (int performances);
  void draw   (int performances);
  void reset  (void);
};
