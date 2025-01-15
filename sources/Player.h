#pragma once

#include "GameObjects.h"
#include "Starship.h"
#include <list>

class Player
{
protected:
  Starship           *mainStarship;
  Starship           *support;
  Time                lastChangeMode;

public:
  static  Player     *player;

  Player(void);

public:
  static  Player &access(void);

public:
  int       getHealth             (void)                { return mainStarship->getHealth(); }
  int       supportCount          (void)                { return support != NULL; }

protected:
  void      moveStarships         (void);

public:
  void      addSupport            (Starship *support);
  void      removeSupport         (void);
  void      changeShootMode       (void);
  void      moveMouse             (void);
  void      leftClick             (bool pressed = true);
  void      rightClick            (bool pressed = true);
  void      spaceBar              (bool pressed = true);
  void      reset                 (void);
};
