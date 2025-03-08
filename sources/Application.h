#pragma once

#include "Player.h"
#include "AsteroidsObjects.h"

/**
 * @enum WindowMode
 * @brief Represents different game states.
 */
enum class WindowMode { GameOver, Game, Menu, Options, Debug };

/**
 * @class Application
 * @brief Manages the main application logic.
 */
class Application
{
protected:
  WindowMode                  currentMode;   ///< Current game mode.
  WindowMode                  lastMode;      ///< Last game mode before change.
  Event                       event;         ///< Current input event.
  int                         difficulty;    ///< Difficulty level.
  int                         performances;  ///< Performance metric.
  string                 currentName;   ///< Current player's name.
  vector<std::pair<int, std::string>> scores; ///< List of top scores.

  GameWindow          *window;           ///< Pointer to game window.
  GameData            *data;             ///< Pointer to game data.
  GameClock           *clock;            ///< Pointer to game clock.
  AsteroidsObjects    *asteroidsObjects; ///< Pointer to game objects.
  Player              *player;           ///< Pointer to the player.

public:
  static  Application *app; ///< Static instance of the application.

  /**
   * @brief Constructor for the Application.
   */
  Application(void);

  /**
   * @brief Gets the singleton instance of Application.
   * @return Reference to the Application instance.
   */
  static Application &application(void);

protected:
  void    initialize        (void); ///< Initializes the game application.
  void    setMode           (const WindowMode &mode); ///< Sets the game mode.
  void    changeScreen      (const WindowMode &mode = WindowMode::Menu); ///< Changes the screen.
  void    quit              (void); ///< Quits the application.
  void    inputManager      (void); ///< Manages user input.
  void    getSavedData      (void); ///< Retrieves saved game data.
  void    saveData          (const std::pair<int, std::string> &data); ///< Saves game data.
  void    update            (void); ///< Updates the game logic.
  void    draw              (void); ///< Draws the game elements.
  
public:
  /**
   * @brief Computes the next game state.
   * @return Integer status code.
   */
  int     compute       (void);
};
