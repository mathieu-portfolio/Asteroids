#include "Player.h"
#include "AsteroidsObjects.h"
#include <iostream>
#include <cmath>
#include "Geometry.h"

using namespace Geometry;


#define minDistMousePlayer 1.f

Player *Player::player = NULL;

Player::Player(void) :
  mainStarship(NULL),
  support     (NULL)
{
  reset();
}

Player &Player::access    (void)
{
  if (player == NULL)
    player = new Player();
  return *player;
}

void Player::changeShootMode (void)
{
  mainStarship->changeShootMode();
}

void Player::addSupport    (Starship *support)
{ 
  this->support = support;
}

void Player::removeSupport (void)
{
  if(support != NULL) {
    support->doDestroy();
    support = NULL;
  }
}

void Player::moveStarships (void)
{
  auto &starships = AsteroidsObjects::objects().getStarships();
  auto  mousePos  = (Vector2f) Mouse::getPosition(GameWindow::window());
  auto  range     = norm((Vector2f) GameWindow::window().getSize()) * 0.25f;

  for(auto *starship : starships) {
    auto &starshipShape = *starship->getShape();
    auto  starshipPos   =  starshipShape.getPosition();
    auto  starshipSpeed =  starship->getSpeed();
  
    if (isInsideWindow(mousePos)) {
      auto maxSpeed = starship->getMaxSpeed();
      auto dist     = norm(mousePos - starshipPos);
      if (dist > range)
        starshipSpeed = maxSpeed;
      else if (dist < minDistMousePlayer)
        starshipSpeed = 0.f;
      else
        starshipSpeed = dist * maxSpeed / range;
    }

    starship->setSpeed(starshipSpeed);


    if (norm(mousePos - starshipPos) > minDistMousePlayer) {
      auto angle = Geometry::angle(
        Vector2fUp,
        mousePos - starshipPos,
        AngleUnit::Degrees
      );

      #ifdef _WIN32
        starshipShape.setRotation(angle);
      #endif
      #ifdef __unix__
        starshipShape.setRotation(degrees(angle));
      #endif
    }
  }
}

void Player::moveMouse   (void)
{
  moveStarships();
  if (support != NULL) {
    support->move(mainStarship->moving());
    support->shoot(mainStarship->shooting());
    if (support->getHealth() < 1)
      removeSupport();
  }
}

void Player::leftClick    (bool pressed)
{
  mainStarship->shoot(pressed);
}

void Player::rightClick   (bool pressed)
{
  mainStarship->move(!mainStarship->moving());
}

void Player::spaceBar     (bool pressed)
{
  mainStarship->changeShootMode();
}

void Player::reset        (void)
{
  if (mainStarship != NULL)
    delete mainStarship;
  mainStarship = new Starship();
  removeSupport();
}

