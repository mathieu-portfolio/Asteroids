#pragma once

#include <SFML/Graphics.hpp>
#include "GameObjects.h"


/**
 * @class Asteroid
 * @brief Represents an asteroid in the game.
 */
class Asteroid : public MovingObject
{
protected:
  int              fragmentation; ///< Number of fragments the asteroid breaks into.
  float            radius; ///< Radius of the asteroid.
  float            minSpeed; ///< Minimum movement speed.
  float            maxSpeed; ///< Maximum movement speed.
  float            maxRadius; ///< Maximum allowed radius.
  vector<Shape *>  holes; ///< Craters or holes on the asteroid surface.

public:
  /**
   * @brief Constructs an asteroid with given parameters.
   */
  Asteroid(
		const Vector2f &position,
    const Vector2f &direction,
    float           minSpeed,
    float           maxSpeed,
    float           maxRadius,
		float           baseRadius        = 160.f,
    int             craterCount       = 0,
    int             fragmentation     = 0,
    const Color    &baseColor         = Color(120, 80, 45, 255)
	);

  /**
   * @brief Alternative constructor without direction.
   */
	Asteroid(
		const Vector2f &position,
    float           minSpeed,
    float           maxSpeed,
    float           maxRadius,
		float           baseRadius        = 160.f,
    int             craterCount       = 0,
    int             fragmentation     = 0,
    const Color    &baseColor         = Color(120, 80, 45, 255)
	);

  /**
   * @brief Destructor.
   */
	virtual ~Asteroid(void);

public:
  float getRadius(void) { return radius; } ///< Gets the asteroid radius.
  const vector<Shape *> &getHoles(void) { return holes; } ///< Gets the list of craters.

protected:
  void addCrater(const Vector2f &craterPosition); ///< Adds a crater at the given position.
	void move(float deltaTime); ///< Moves the asteroid.
  void destroy(void); ///< Handles asteroid destruction.

public:
  bool isInside(const Shape &subjectShape); ///< Checks if a shape is inside the asteroid.
  void fragment(int performances); ///< Handles fragmentation upon destruction.

	void update(int performances); ///< Updates asteroid state.
  void draw(int performances); ///< Draws the asteroid.
};
