#include "AsteroidsObjects.h"
#include "Player.h"
#include <iostream>
#include "Geometry.h"
#include <limits>

using namespace Geometry;

constexpr float fInfinity = numeric_limits<float>::max();

#define invincibleDuration 3.f

Starship::Starship (void) :
  Starship(
    0.5f * (Vector2f) GameWindow::window().getSize(),
    40.f,
    1000.f,
    3
  )
{
}

Starship::Starship(
  const Vector2f &position,
  float           radius,
  float           maxSpeed,
  int             health
) :
  MovingObject(
    radius,
    3,
    position,
    Vector2fUp,
    0.f,
    0.f,
    Color(100, 255, 100, 255),
    milliseconds(1000)
  ),
  health   (health),
  maxSpeed (maxSpeed)
{
  reset();
  AsteroidsObjects::objects().getStarships().add(this);
}

Starship::~Starship (void)
{
  AsteroidsObjects::objects().getStarships().remove(this);
}


void Starship::shoot        (void)
{
  auto &clock          = GameClock::clock();
  auto  currentUpdate  = clock.getCurrentUpdate();
  auto  deltaUpdate    = clock.getDeltaUpdate().asSeconds();
  auto  timeSinceStart = (currentUpdate - clock.getStartingTime()).asSeconds();
  auto  timeLimit      = 4.f;
  if (speed < 0.3f * maxSpeed)
    movingTime = max(
      0.f,
      movingTime - 3 * deltaUpdate
    );
  else
    movingTime = min(
      timeLimit,
      movingTime + deltaUpdate
    );

  if (isShooting) {
    auto timeSinceShot = (currentUpdate - lastShot).asSeconds();

    auto maxBoost      = 0.3f;
    auto cadencyMovingBoost = max(
      0.5f,
      1 - movingTime / timeLimit
    );
    auto cadencyTimeBoost   = max(
      0.6f,
      1.f - timeSinceStart / 120.f
    );
    auto cadencyBoost       = max(
      maxBoost,
      cadencyMovingBoost * cadencyTimeBoost
    );

    auto finalCadency = shootingCadency;
    if (boost == PowerUpBoost::Cadency)
      finalCadency *= 0.5f;

    switch(shootmode) {
      case (ShootMode::Fast):
        if (timeSinceShot > finalCadency * cadencyBoost) {
          new Bullet(
            this,
            shape->getPosition() + ((CircleShape *) shape)->getRadius() * direction,
            direction,
            speed + 1000.f,
            5.f
          );

          lastShot = currentUpdate;
        }
        break;

      case (ShootMode::Slow):
        if (timeSinceShot > finalCadency) {
          new Bullet(
            this,
            shape->getPosition() + ((CircleShape *) shape)->getRadius() * direction,
            direction,
            100.f,
            30.f
          );

          lastShot = currentUpdate;
        }
        break;

      default:
        break;
    }
  }
}

void Starship::move         (float deltaTime)
{
  if (isMoving) {
    if (boost == PowerUpBoost::Movement)
      speed *= 2.f;
    MovingObject::move(deltaTime);
  }
}

void Starship::rotate       (void)
{
  direction = Geometry::rotate(
    Vector2fUp,
    #ifdef _WIN32
      shape->getRotation(),
    #else
      shape->getRotation().asDegrees(),
    #endif
    AngleUnit::Degrees
  );
}

bool Starship::takeDamage   (void)
{
  auto    currentUpdate       = GameClock::clock().getCurrentUpdate();
  auto    destroyPeriod       = 0.15f;
  auto    timeSinceInvicible  = (currentUpdate -  lastBoost).asSeconds();
  auto    timeSinceDamage     = (currentUpdate - lastDamage).asSeconds();
  auto    invincible          = boost == PowerUpBoost::Invincibility && timeSinceInvicible < powerupTime;

  // unables to destroy asteroids while invicible
  if (invincible)
    if (timeSinceDamage < destroyPeriod)
      return false;

  // iterate over asteroids
  auto playerShape = *(CircleShape *) shape;
  auto position    = playerShape.getPosition();
  auto radius      = playerShape.getRadius();
  for(auto *asteroid : AsteroidsObjects::objects().getAsteroids()) {
    auto asteroidShape  = *(ConvexShape *) asteroid->getShape();
    auto asteoridCenter = asteroidShape.getPosition();
    auto asteoridRadius = asteroid->getRadius();
    // if starship is close enough, check in more details
    if (norm(asteoridCenter - position) < 1.5f * asteoridRadius + radius) {

      // iterate over starship shape points
      for(auto i = 0; i < (int) shape->getPointCount(); i++) {
        // includes rotation of the object
        auto point = shape->getTransform().transformPoint(shape->getPoint(i));
        // check if point is inside a hole
        bool inHole = false;
        for(auto *hole : asteroid->getHoles()) {
          if (isInside(point, *hole)) {
            inHole = true;
            break;
          }
        }
        // check the if point is inside the asteroid
        bool inAsteroid = isInside(point, asteroidShape);
        
        // if the point is inside the asteroid and not inside a hole, take damage
        if(inAsteroid && !inHole) {
          asteroid->doDestroy();
          lastDamage = currentUpdate;
          return !invincible;
        }
      }
    }
  }

  return false;
}

void Starship::damageManager(void)
{
  if (takeDamage()) {
    health--;
    changeBoost(PowerUpBoost::Invincibility);
  }
}

void Starship::powerupManager(void)
{
  if (boost == PowerUpBoost::None)
    return;

  auto &clock = GameClock::clock();
  auto timeSinceBoost = (clock.getCurrentUpdate() - lastBoost).asSeconds();

  if(timeSinceBoost < powerupTime) {
    twinkle(boostColor, baseColor);
  }
  else {
    shape->setFillColor(baseColor);
    changeBoost(PowerUpBoost::None);
  }
}

void Starship::changeBoost       (const PowerUpBoost newBoost)
{
  auto &player = Player::access();
  if (boost == PowerUpBoost::Support)
    player.removeSupport();


  switch(newBoost) {
    case (PowerUpBoost::None):
      powerupTime = 0.f;
      boostColor  = baseColor;
      break;

    case (PowerUpBoost::Movement):
      powerupTime = 8.f;
      boostColor  = Color(230, 255, 230);
      break;

    case (PowerUpBoost::Cadency):
      powerupTime = 6.f;
      boostColor  = Color(255, 200, 255);
      break;

    case (PowerUpBoost::Life):
      health++;
      powerupTime = 0.3f;
      boostColor  = Color(255, 200, 200);
     break;

    case (PowerUpBoost::Invincibility):
      powerupTime = 3.f;
      boostColor  = Color(255, 255, 200);
      break;

    case (PowerUpBoost::Support):
      if (!player.supportCount())
      player.addSupport(new Starship(
        shape->getPosition()
      ));
      powerupTime = 10.f;
      boostColor  = baseColor;
      break;

    default:
      break;
  }

  boost = newBoost;
  auto currentUpdate = GameClock::clock().getCurrentUpdate();
  lastBoost  = currentUpdate;
  lastDamage = currentUpdate;
  twinkleClock.reset();
}

void Starship::setShootMode   (const ShootMode mode)
{
  shootmode = mode;
  switch (shootmode) {
    case (ShootMode::Fast):
      shootingCadency = 0.2f;
      break;

    case (ShootMode::Slow):
      shootingCadency = 0.2f;
      break;

    default:
      break;
  }
}

void Starship::changeShootMode(void)
{
  auto currentUpdate       = GameClock::clock().getCurrentUpdate();
  auto timeSinceChangeMode = (currentUpdate - lastChangeMode).asSeconds();
  if (timeSinceChangeMode > 0.5f) {
    setShootMode((ShootMode) (((int) shootmode + 1) % 2));
    lastChangeMode = currentUpdate;
  }
}

void Starship::update       (int performances)
{
  MovingObject::update(performances);
  rotate();
  shoot();
  damageManager();
  powerupManager();
}

void Starship::draw         (int performances)
{
  RenderedObject::draw(performances);
}

void Starship::reset        (void)
{
  auto currentUpdate    = GameClock::clock().getCurrentUpdate();
  shape->setFillColor   (baseColor);
  setShootMode          ();
  direction             = Vector2fUp;
  movingTime            = 0.f;
  powerupTime           = 0.f;
  lastShot              = currentUpdate - seconds(shootingCadency);
  lastBoost             = currentUpdate;
  lastDamage            = currentUpdate - seconds(fInfinity);
  lastChangeMode        = currentUpdate;
  boost                 = PowerUpBoost::None;
  isShooting            = false;
  isMoving              = false;
  boostColor            = baseColor;
}




Starships::Starships  (void)
{
}

void Starships::update (int performances)
{
  ObjectList<Starship>::update(performances);
}

void Starships::draw (int performances)
{
  ObjectList<Starship>::draw(performances);
}