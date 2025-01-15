#pragma once

#include <SFML/Graphics.hpp>
#include "GameObjects.h"


class Asteroid : public MovingObject
{
protected:
  int              fragmentation;
  float            radius;
  float            minSpeed;
  float            maxSpeed;
  float            maxRadius;
  vector<Shape *>  holes;

public:
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
	virtual ~Asteroid(void);

public:
  float                   getRadius   (void)        { return radius; }
  const vector<Shape *>  &getHoles    (void)        { return holes; }

protected:
  void  addCrater   (const Vector2f &craterPosition);
	void  move        (float deltaTime);
  void  destroy     (void);

public:
  bool  isInside    (const Shape &subjectShape);
  void  fragment    (int performances);

	void  update      (int performances);
  void  draw        (int performances);
};
