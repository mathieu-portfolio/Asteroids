#include "GameData.h"


const Vector2f  spawnOutOfWindow  (const Vector2f &size)
{
  Vector2f res;

  // generate outside frame boudaries
  auto  windowSize = GameWindow::window().getSize();
  switch (rand() % 4) {
    case 0:
      res.x = (float) (rand() % (int) windowSize.x);
      res.y = - 0.5f * size.y;
      break;

    case 1:
      res.x = (float) (rand() % (int) windowSize.x);
      res.y = windowSize.y + 0.5f * size.y;
      break;

    case 2:
      res.x = - 0.5f * size.x;
      res.y = (float) (rand() % (int) windowSize.y);
      break;

    case 3:
      res.x = windowSize.x + 0.5f * size.x;
      res.y = (float) (rand() % (int) windowSize.y);
      break;
  }

  return res;
}

const Vector2f spawnInsideWindow  (const Vector2f &size)
{
  // generate a random position inside the frame boundaries
  auto windowSize = GameWindow::window().getSize();
  auto random_x = (float) (rand() % (int) (windowSize.x - size.x) + 0.5f * size.x);
  auto random_y = (float) (rand() % (int) (windowSize.y - size.y) + 0.5f * size.y);
  return Vector2f(random_x, random_y);
}

const bool isInsideWindow         (const Vector2f &v, float radius)
{
  auto windowSize = GameWindow::window().getSize();
  if (v.x + radius < 0)
    return false;
  if (v.x - radius > windowSize.x)
    return false;
  if (v.y + radius < 0)
    return false;
  if (v.y - radius > windowSize.y)
    return false;
  
  return true;
}



GameWindow *GameWindow::gameWindow = NULL;

GameWindow &GameWindow::window    (void)
{
  if (gameWindow == NULL)
    gameWindow = new GameWindow();
  return *gameWindow;
}

GameWindow::GameWindow            (void) :
  #ifdef _WIN32
    RenderWindow(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Asteroids", Style::Close | Style::Fullscreen)
  #else
    RenderWindow(VideoMode(Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)), "Asteroids", Style::Close | Style::Fullscreen)
  #endif
{
  auto windowSize = getSize();
  #ifdef _WIN32
  if (!renderTexture.create(windowSize.x, windowSize.y))
    perror("error texture");
  #else
  if (!renderTexture.create(Vector2u(windowSize.x, windowSize.y)))
    perror("error texture");
  #endif
}



GameData *GameData::gameData = NULL;

GameData::GameData                (void)
{
  reset();
}

GameData &GameData::data          (void)
{
  if (gameData == NULL)
    gameData = new GameData();
  return *gameData;
}

void GameData::reset              (void)
{
  score         = 0;
  difficulty    = 1;
}
