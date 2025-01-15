#pragma once

#include <SFML/Graphics.hpp>
#include "GameObjects.h"
#include "GameData.h"


class PowerUp: public RenderedObject
{
protected:
  PowerUpBoost  boost;

public:
  PowerUp(
    const Vector2f      &position,
    const Vector2f      &size      = Vector2f(200.f, 100.f),
    const PowerUpBoost   baseBoost = PowerUpBoost::None
  );
  virtual ~PowerUp(void);

public:
  PowerUpBoost  getBoost (void)  { return boost; }

protected:
  void          taken    (void);

public:
  void          update   (int performances);
  void          draw     (int performances);
};
