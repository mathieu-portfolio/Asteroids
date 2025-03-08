#pragma once

#include <SFML/Graphics.hpp>
#include "GameObjects.h"
#include "GameData.h"

/**
 * @class PowerUp
 * @brief Represents a collectible power-up in the game.
 */
class PowerUp: public RenderedObject
{
protected:
  PowerUpBoost boost; ///< The type of power-up effect.

public:
  /**
   * @brief Constructs a PowerUp object.
   * @param position The spawn position of the power-up.
   * @param size The size of the power-up.
   * @param baseBoost The type of boost provided.
   */
  PowerUp(
    const Vector2f &position,
    const Vector2f &size = Vector2f(200.f, 100.f),
    const PowerUpBoost baseBoost = PowerUpBoost::None
  );

  /**
   * @brief Destructor.
   */
  virtual ~PowerUp(void);

public:
  /**
   * @brief Gets the type of boost this power-up provides.
   * @return The PowerUpBoost type.
   */
  PowerUpBoost getBoost(void) { return boost; }

protected:
  /**
   * @brief Handles the event when the power-up is collected.
   */
  void taken(void);

public:
  /**
   * @brief Updates the power-up logic.
   * @param performances Performance factor for update timing.
   */
  void update(int performances);

  /**
   * @brief Draws the power-up on the screen.
   * @param performances Performance factor for rendering.
   */
  void draw(int performances);
};
