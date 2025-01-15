#include "AsteroidsObjects.h"



AsteroidsObjects *AsteroidsObjects::asteroidsObjects = NULL;

AsteroidsObjects &AsteroidsObjects::objects(void)
{
  if (asteroidsObjects == NULL)
    asteroidsObjects = new AsteroidsObjects();
  return *asteroidsObjects;
}

AsteroidsObjects::AsteroidsObjects(void)
{
}

void AsteroidsObjects::update (int performances)
{

  asteroids.update(performances);
  bullets.update(performances);
  powerUps.update(performances);
  starships.update(performances);
}

void AsteroidsObjects::draw (int performances)
{
  GameObjects::objects().draw(performances);
}

void AsteroidsObjects::reset(void)
{
  asteroids = Asteroids();
  bullets   = Bullets();
  powerUps  = PowerUps();
  starships = Starships();
  clear();
}

