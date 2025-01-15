#pragma once

#include <SFML/Graphics.hpp>
#include "GameObjects.h"
#include "Starship.h"


class Bullet: public MovingObject
{
protected:
  Starship *source;

public:
  Bullet(
          Starship *starship,
    const Vector2f &position,
    const Vector2f &direction,
    float           baseSpeed   = 1000.f,
    float           radius      = 10.f
  );
  virtual ~Bullet   (void);

public:
  Starship &getSource (void) { return *source; }

protected:
  void    move      (float deltaTime);

public:
  void    update    (int performances);
};

