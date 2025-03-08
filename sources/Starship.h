#pragma once

#include <SFML/Graphics.hpp>
#include "GameObjects.h"
#include "GameData.h"

/**
 * @class Starship
 * @brief Represents a controllable starship.
 */
class Starship: public MovingObject
{
protected:
  int health; ///< The health of the starship.
  float maxSpeed; ///< The maximum movement speed.
  float movingTime; ///< Total time spent moving.
  float powerupTime; ///< Duration of power-up effect.
  float shootingCadency; ///< The rate of fire.
  bool isShooting; ///< Whether the starship is currently shooting.
  bool isMoving; ///< Whether the starship is moving.

  Time lastShot; ///< Time of last shot fired.
  Time lastBoost; ///< Time of last power-up usage.
  Time lastDamage; ///< Time of last damage taken.
  Time lastChangeMode; ///< Time of last shooting mode change.

  ShootMode shootmode; ///< Current shooting mode.
  PowerUpBoost boost; ///< Current active power-up.
  Color boostColor; ///< Visual indicator for power-up effect.

public:
  /**
   * @brief Default constructor.
   */
  Starship(void);

  /**
   * @brief Constructs a Starship with given parameters.
   * @param position Starting position.
   * @param radius The size of the starship.
   * @param maxSpeed Maximum movement speed.
   * @param health Initial health value.
   */
  Starship(
    const Vector2f &position,
    float radius = 20.f,
    float maxSpeed = 2000.f,
    int health = 1
  );

  /**
   * @brief Destructor.
   */
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
  void shoot(void); ///< Handles shooting logic.
  void move(float deltaTime); ///< Moves the starship.
  void rotate(void); ///< Handles ship rotation.
  bool takeDamage(void); ///< Handles taking damage.
  void damageManager(void); ///< Manages damage effects.
  void powerupManager(void); ///< Manages power-up effects.

public:
  /**
   * @brief Changes the current power-up effect.
   * @param newBoost The new PowerUpBoost.
   */
  void changeBoost(const PowerUpBoost newBoost);

  /**
   * @brief Sets the shooting mode.
   * @param mode The new shooting mode (default: Fast).
   */
  void setShootMode(const ShootMode mode = ShootMode::Fast);

  /**
   * @brief Toggles between shooting modes.
   */
  void changeShootMode(void);

  /**
   * @brief Updates the starship.
   * @param performances Performance factor.
   */
  void update(int performances);

  /**
   * @brief Draws the starship.
   * @param performances Performance factor.
   */
  void draw(int performances);

  /**
   * @brief Resets the starship state.
   */
  void reset(void);
};
