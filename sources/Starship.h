#pragma once

#include <SFML/Graphics.hpp>
#include "GameObjects.h"
#include "GameData.h"


class Starship: public MovingObject
{
protected:
  int           health;
  float         maxSpeed;
  float         movingTime;
  float         powerupTime;
  float         shootingCadency;
  bool          isShooting;
  bool          isMoving;
  Time          lastShot;
  Time          lastBoost;
  Time          lastDamage;
  Time          lastChangeMode;
  ShootMode     shootmode;
  PowerUpBoost  boost;
  Color         boostColor;

public:
  Starship(void);
  Starship(
    const Vector2f &position,
    float           radius    = 20.f,
    float           maxSpeed  = 2000.f,
    int             health    = 1
  );
  virtual ~Starship(void);

public:
  int           getHealth               (void)                          { return health; }
  void          setHealth               (int newHealth)                 { health = newHealth; }
  void          shoot                   (bool shoot)                    { isShooting = shoot; }
  bool          shooting                (void)                          { return isShooting; }
  void          move                    (bool move)                     { isMoving = move; }
  bool          moving                  (void)                          { return isMoving; }
  float         getMaxSpeed             (void)                          { return maxSpeed; }
  void          setMaxSpeed             (float newSpeed)                { maxSpeed = newSpeed; }
  PowerUpBoost  getBoost                (void)                          { return boost; }
  float         getBoostTime            (void)                          { return powerupTime; }

protected:
  void          shoot                   (void);
  void          move                    (float deltaTime);
  void          rotate                  (void);
  bool          takeDamage              (void);
  void          damageManager           (void);
  void          powerupManager          (void);

public:
  void          changeBoost             (const PowerUpBoost newBoost);
  void          setShootMode            (const ShootMode mode = ShootMode::Fast);
  void          changeShootMode         (void);
  void          update                  (int performances);
  void          draw                    (int performances);
  void          reset                   (void);
};
