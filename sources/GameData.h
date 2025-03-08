#pragma once

#include <SFML/Graphics.hpp>

using namespace sf;

#define WINDOW_WIDTH  1000  ///< Default window width.
#define WINDOW_HEIGHT 1000  ///< Default window height.

/// @brief Rectangle representing the game window.
const RectangleShape windowRect(Vector2f(WINDOW_WIDTH, WINDOW_WIDTH));

/**
 * @brief Spawns an object outside the game window.
 * @param size Size of the object.
 * @return Position vector outside the window.
 */
const Vector2f spawnOutOfWindow(const Vector2f &size);

/**
 * @brief Spawns an object inside the game window.
 * @param size Size of the object.
 * @return Position vector inside the window.
 */
const Vector2f spawnInsideWindow(const Vector2f &size);

/**
 * @brief Checks if a position is inside the game window.
 * @param v Position vector.
 * @param radius Optional radius for collision checks.
 * @return True if inside window, false otherwise.
 */
const bool isInsideWindow(const Vector2f &v, float radius = 0.f);

/**
 * @enum ShootMode
 * @brief Defines shooting modes for the player.
 */
enum class ShootMode {
  Fast = 0, ///< Fast shooting mode.
  Slow = 1  ///< Slow shooting mode.
};

/**
 * @enum PowerUpBoost
 * @brief Defines power-up types.
 */
enum class PowerUpBoost 
{
  None = 0,           ///< No power-up effect.
  Movement = 1,       ///< Increases movement speed.
  Cadency = 2,        ///< Increases shooting speed.
  Life = 3,           ///< Grants extra life.
  Invincibility = 4,  ///< Grants temporary invincibility.
  Support = 5         ///< Summons AI support.
};

/**
 * @class GameWindow
 * @brief Represents the main game window.
 */
class GameWindow : public RenderWindow 
{
protected:
  RenderTexture renderTexture; ///< Render texture for offscreen rendering.

public:
  static GameWindow *gameWindow; ///< Singleton instance of GameWindow.

  /**
   * @brief Constructs the GameWindow.
   */
  GameWindow(void);

  /**
   * @brief Gets the singleton instance of GameWindow.
   * @return Reference to the GameWindow instance.
   */
  static GameWindow &window(void);

  RenderTexture &getRenderTexture(void) { return renderTexture; }
};


/**
 * @class GameData
 * @brief Stores global game data.
 */
class GameData
{
protected:
  int score; ///< Current player score.
  int difficulty; ///< Current game difficulty level.

public:
  static GameData *gameData; ///< Singleton instance of GameData.

  /**
   * @brief Constructs the GameData object.
   */
  GameData(void);

  /**
   * @brief Gets the singleton instance of GameData.
   * @return Reference to the GameData instance.
   */
  static GameData &data(void);

public:
  int           getScore        (void)                    { return score; }
  void          setScore        (int newScore)            { score = newScore; }
  void          addScore        (int value)               { score += value; }
  int           getDifficulty   (void)                    { return difficulty; }
  void          setDifficulty   (int newDifficulty)       { difficulty = newDifficulty; }
  void          addDifficulty   (int value)               { difficulty += value; }

  /**
   * @brief Resets the game data.
   */
  void    reset         (void);
};
