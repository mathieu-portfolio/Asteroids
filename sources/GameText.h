#pragma once

#include <SFML/Graphics.hpp>
#include "GameList.h"

using namespace sf;
using namespace std;


/**
 * @enum TextBorder
 * @brief Defines text alignment relative to borders.
 */
enum class TextBorder {
  Left    = 0, ///< Align to left.
  Right   = 1, ///< Align to right.
  Top     = 2, ///< Align to top.
  Bottom  = 3  ///< Align to bottom.
};

/**
 * @class GameText
 * @brief Represents text elements used in the game.
 */
class GameText: public Text
{
public:
  /**
   * @brief Constructs a GameText object.
   * @param content Text content.
   * @param size Font size.
   * @param color Text color.
   */
  GameText(
    const string &content,
    int           size,
    const Color  &color = Color::White
  );

public:
  void center(
    float x,
    float y,
    float xOffset,
    float yOffset
  );
  void center(
    float x,
    float y,
    const Vector2f &offset   = Vector2f(0.f,  0.f)
  );
  void center(
    const Vector2f &position = Vector2f(0.5f, 0.5f),
    const Vector2f &offset   = Vector2f(0.f,  0.f)
  );

  void centerContent(
    float x,
    float y
  );
  void centerContent(
    const Vector2f &origin = Vector2f(0.5f, 0.5f)
  );
  
  Vector2f positionRelativeToWindow (void);
  Vector2f originRelativeToBounds   (void);

  void alignTo(
    GameText          text,
    float             xOffset,
    float             yOffset,
    const TextBorder &border            = TextBorder::Bottom,
    bool              keepWindowCenter  = true,
    bool              keepContentCenter = true
  );
  void alignTo(
    GameText          text,
    const TextBorder &border            = TextBorder::Bottom,
    bool              keepPosition      = true,
    bool              keepOrigin        = true,
    const Vector2f   &offset            = Vector2f(0.f, 0.f)
  );

  void update(void);
  void draw  (void);
};

/**
 * @class TextList
 * @brief Manages a list of text elements in the game.
 */
class TextList : public GameList<GameText>
{
public:
  /**
   * @brief Constructs a TextList object.
   */
  TextList(void);

public:
  void update(void);
  void draw(void);
};