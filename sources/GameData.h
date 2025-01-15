#pragma once

#include <SFML/Graphics.hpp>

using namespace sf;

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 1000

const RectangleShape windowRect        (Vector2f(WINDOW_WIDTH, WINDOW_WIDTH));
const Vector2f       spawnOutOfWindow  (const Vector2f &size);
const Vector2f       spawnInsideWindow (const Vector2f &size);
const bool           isInsideWindow    (const Vector2f &v, float radius = 0.f);


enum class ShootMode {
  Fast = 0,
  Slow = 1
};

enum class PowerUpBoost 
{
  None              = 0,
  Movement          = 1,
  Cadency           = 2,
  Life              = 3,
  Invincibility     = 4,
  Support           = 5
};


class GameWindow : public RenderWindow 
{
protected:
  RenderTexture renderTexture;

public:
  static GameWindow *gameWindow;

  GameWindow(void);

  static  GameWindow      &window               (void);

public:
          RenderTexture   &getRenderTexture     (void)      { return renderTexture; }
};


class GameData
{
protected:
  int         score;
  int         difficulty;

public:
  static GameData *gameData;

  GameData(void);

  static GameData &data(void);

public:
  int           getScore        (void)                    { return score; }
  void          setScore        (int newScore)            { score = newScore; }
  void          addScore        (int value)               { score += value; }
  int           getDifficulty   (void)                    { return difficulty; }
  void          setDifficulty   (int newDifficulty)       { difficulty = newDifficulty; }
  void          addDifficulty   (int value)               { difficulty += value; }

public:
  void    reset         (void);
};
