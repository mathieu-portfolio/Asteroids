#include "AsteroidsObjects.h"
#include <iostream>
#include "Geometry.h"

using namespace Geometry;


Bullet::Bullet(
        Starship *starship,
  const Vector2f &position,
  const Vector2f &direction,
  float           baseSpeed,
  float           radius
) :
  MovingObject(
    radius,
    10,
    position,
    direction,
    baseSpeed,
    0.f,
    Color(255, 200, 255, 255)
  ),
  source(starship)
{
  AsteroidsObjects::objects().getBullets().add(this);
}

Bullet::~Bullet     (void)
{
  AsteroidsObjects::objects().getBullets().remove(this);
}

void Bullet::move   (float deltaTime)
{
  MovingObject::move(deltaTime);
}

void Bullet::update (int performances)
{
  MovingObject::update(performances);
  auto radius = ((CircleShape *) shape)->getRadius();
  RectangleShape largerRect = RectangleShape((Vector2f) GameWindow::window().getSize() + 2 * radius * Vector2fOne);
  largerRect.move(-radius * Vector2fOne);
  if (!isInside(shape->getPosition(), largerRect))
    destroyed = true;
}


Bullets::Bullets          (void)
{
}
