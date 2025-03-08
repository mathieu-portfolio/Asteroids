#pragma once

#include <SFML/Graphics.hpp>
#include "GameObjects.h"
#include "Starship.h"

/**
 * @class Bullet
 * @brief Represents a bullet fired by a starship.
 */
class Bullet: public MovingObject
{
protected:
  Starship *source; ///< The starship that fired the bullet.

public:
  /**
   * @brief Constructs a bullet.
   * @param starship The source starship.
   * @param position Starting position.
   * @param direction Direction of travel.
   * @param baseSpeed Initial speed.
   * @param radius Bullet size.
   */
  Bullet(
          Starship *starship,
    const Vector2f &position,
    const Vector2f &direction,
    float           baseSpeed   = 1000.f,
    float           radius      = 10.f
  );

  /**
   * @brief Destructor.
   */
  virtual ~Bullet(void);

public:
  /**
   * @brief Gets the starship that fired the bullet.
   * @return Reference to the source starship.
   */
  Starship &getSource(void) { return *source; }

protected:
  void move(float deltaTime); ///< Moves the bullet.

public:
  void update(int performances); ///< Updates bullet behavior.
};
