#pragma once

#include "GameObjects.h"
#include "Starship.h"
#include <list>

/**
 * @class Player
 * @brief Represents the player controlling a starship.
 */
class Player
{
protected:
  Starship *mainStarship; ///< Main starship controlled by the player.
  Starship *support; ///< Optional support starship.
  Time lastChangeMode; ///< Timestamp of the last mode change.

public:
  static Player *player; ///< Singleton instance of the Player.

  /**
   * @brief Constructs a Player object.
   */
  Player(void);

public:
  /**
   * @brief Gets the singleton instance of the Player.
   * @return Reference to the Player instance.
   */
  static Player &access(void);

public:
  /**
   * @brief Gets the player's remaining health.
   * @return Current health value.
   */
  int getHealth(void) { return mainStarship->getHealth(); }

  /**
   * @brief Checks if the player has a support starship.
   * @return 1 if support exists, otherwise 0.
   */
  int supportCount(void) { return support != NULL; }

protected:
  /**
   * @brief Moves the player's starships.
   */
  void moveStarships(void);

public:
  /**
   * @brief Adds a support starship.
   * @param support Pointer to the support starship.
   */
  void addSupport(Starship *support);

  /**
   * @brief Removes the support starship.
   */
  void removeSupport(void);

  /**
   * @brief Changes the shooting mode of the player.
   */
  void changeShootMode(void);

  /**
   * @brief Handles player movement via mouse input.
   */
  void moveMouse(void);

  /**
   * @brief Handles left mouse click actions.
   * @param pressed Whether the button is pressed.
   */
  void leftClick(bool pressed = true);

  /**
   * @brief Handles right mouse click actions.
   * @param pressed Whether the button is pressed.
   */
  void rightClick(bool pressed = true);

  /**
   * @brief Handles space bar actions.
   * @param pressed Whether the space bar is pressed.
   */
  void spaceBar(bool pressed = true);

  /**
   * @brief Resets the player's state.
   */
  void reset(void);
};
