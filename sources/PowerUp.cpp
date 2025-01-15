#include "AsteroidsObjects.h"
#include "Geometry.h"
#include <iostream>

using namespace Geometry;


PowerUp::PowerUp(
  const Vector2f      &position,
  const Vector2f      &size,
  const PowerUpBoost   baseBoost
) :
  RenderedObject(
    size,
    position,
    Color::Magenta,
    milliseconds(2000)
  ),
  boost     (baseBoost)
{
  if(boost == PowerUpBoost::None) {
    boost = (PowerUpBoost) (rand() % 5 + 1);
  }
  AsteroidsObjects::objects().getPowerUps().add(this);

  switch (boost) {
    case (PowerUpBoost::Movement):
      baseColor = Color(190, 255, 190);
      break;

    case (PowerUpBoost::Cadency):
      baseColor = Color(190, 255, 255);
      break;

    case (PowerUpBoost::Life):
      baseColor = Color(255, 190, 190);
      break;

    case (PowerUpBoost::Invincibility):
      baseColor = Color(255, 255, 190);
      break;

    case (PowerUpBoost::Support):
      baseColor = Color(190, 190, 255);
      break;

    default:
      break;
  }
}

PowerUp::~PowerUp     (void)
{
  AsteroidsObjects::objects().getPowerUps().remove(this);
}

void PowerUp::taken   (void)
{
  auto rectShape  = *(RectangleShape *) shape; 
  auto radius     = norm(rectShape.getSize());
  auto position   = shape->getPosition();

  auto &bullets = AsteroidsObjects::objects().getBullets();

  for(auto *bullet : bullets) {
    auto bulletShape  = *(CircleShape *) bullet->getShape();
    auto bulletCenter = bulletShape.getPosition();
    if(norm(bulletCenter - position) < 1.5f * radius + bulletShape.getRadius()) {
      if(isInside(rectShape, bulletShape) || isInside(bulletShape, rectShape)) {
        destroyed = true;
        auto &starship = bullet->getSource();
        starship.changeBoost(boost);
        bullet->doDestroy();
        return;
      }
    }
  }

  auto &starships = AsteroidsObjects::objects().getStarships();

  for(auto *starship : starships) {
    auto playerShape  = *(CircleShape *) starship->getShape();
    auto playerCenter = playerShape.getPosition();
    auto playerRadius = playerShape.getRadius();
    if (norm(position - playerCenter) < 1.5f * radius + playerRadius) {
      if(isInside(rectShape, playerShape) || isInside(playerShape, rectShape)) {
        destroyed = true;
        starship->changeBoost(boost);
        return;
      }
    }
  }
}

void PowerUp::update  (int performances)
{
  taken();
  twinkle();
}

void PowerUp::draw    (int performances)
{
  RenderedObject::draw(performances);
}




PowerUps::PowerUps    (void)
{
  reset();
}

void PowerUps::spawn  (int performances)
{
  if (performances == 0)
    return;

  auto &clock = GameClock::clock();
  auto currentTime =  clock.getCurrentUpdate();
  auto timeSinceSpawn = (currentTime - lastSpawn).asSeconds();

  if(timeSinceSpawn > spawnTime) {
    auto powerupSize = Vector2f(80.f, 50.f);
    auto powerupPos  = spawnInsideWindow(powerupSize);
    new PowerUp(
      powerupPos,
      powerupSize
    );
    lastSpawn = currentTime;
  }
}

void PowerUps::update (int performances)
{
  spawn(performances);
  ObjectList<PowerUp>::update(performances);
}

void PowerUps::reset (void)
{
  auto &clock = GameClock::clock();
  auto currentTime =  clock.getCurrentUpdate();
  spawnTime = 10.f;
  lastSpawn = currentTime - seconds(spawnTime);
}
