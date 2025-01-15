#include "GameText.h"
#include "GameData.h"
#include <iostream>
#include "Geometry.h"

using namespace Geometry;


GameText::GameText(
  const string &content,
  int           size,
  const Color  &color
)
{
  setString(content);
  setCharacterSize(size);
  setFillColor(color);
}

void GameText::center(
  float x,
  float y,
  float xOffset,
  float yOffset
)
{
  auto windowSize = GameWindow::window().getSize();
  x = max(0.f, min(x, 1.f));
  y = max(0.f, min(y, 1.f));
  auto position = Vector2f(x * windowSize.x + xOffset, y * windowSize.y + yOffset);
  setPosition(position);
}

void GameText::center(
  float x,
  float y,
  const Vector2f &offset
)
{
  center(x, y, offset.x, offset.y);
}

void GameText::center(
  const Vector2f &position,
  const Vector2f &offset
)
{
  center(offset.x, offset.y, position);
}

void GameText::centerContent(
  float x,
  float y
)
{
  auto bounds = getLocalBounds();
  x = max(0.f, min(x, 1.f));
  y = max(0.f, min(y, 1.f));
  auto origin = Vector2f(x * bounds.width, y * bounds.height);
  setOrigin(origin);
}

void GameText::centerContent(
  const Vector2f &c
)
{
  centerContent(c.x, c.y);
}

Vector2f GameText::positionRelativeToWindow (void)
{
  auto windowSize = (Vector2f) GameWindow::window().getSize();
  auto position   = getPosition();
  auto x = 0.f;
  auto y = 0.f;
  if (windowSize.x != 0)
    x = position.x / windowSize.x;
  if (windowSize.y != 0)
    y = position.y / windowSize.y;
  return Vector2f(x, y);
}

Vector2f GameText::originRelativeToBounds (void)
{
  auto bounds    = getLocalBounds();
  auto origin    = getOrigin();
  auto x = 0.f;
  auto y = 0.f;
  if (bounds.width != 0.f)
    x = origin.x / bounds.width;
  if (bounds.height != 0.f)
    y = origin.y / bounds.height;
  return Vector2f(x, y);
}

void GameText::alignTo(
  GameText          text,
  const TextBorder &border,
  bool              keepPosition,
  bool              keepOrigin,
  const Vector2f   &offset
)
{
  auto refPosition  = text.getPosition();
  auto refOrigin    = text.originRelativeToBounds();
  auto refBounds    = text.getLocalBounds();
  Vector2f position;
  Vector2f origin;
  if (keepPosition)
    position = refPosition;
  if (keepOrigin)
    origin = refOrigin;

  if(border == TextBorder::Left) {
    origin.x = 1.f;
    position.x += refPosition.x;
  }
  else if(border == TextBorder::Right) {
    origin.x = 0.f;
    position.x += refPosition.x + refBounds.width;
  }
  else if(border == TextBorder::Top) {
    origin.y = 1.f;
    position.y += refPosition.y;
  }
  else if(border == TextBorder::Bottom) {
    origin.y = 0.f;
    position.y += refBounds.height;
  }
  setPosition(position + offset);
  centerContent(origin);
}

void GameText::alignTo(
  GameText          text,
  float             xOffset,
  float             yOffset,
  const TextBorder &border,
  bool              keepWindowCenter,
  bool              keepContentCenter
)
{
  alignTo(text, border, keepWindowCenter, keepContentCenter, Vector2f(xOffset, yOffset));
}

void GameText::update(void)
{
}

void GameText::draw  (void)
{
  auto &window = GameWindow::window();
  window.draw(*this);
}


TextList::TextList    (void)
{
}

void TextList::update (void)
{
  for (auto textIt = GameList<GameText>::begin(); textIt != GameList<GameText>::end(); ) {
    auto nextTextIt = textIt;
    nextTextIt++;
    auto *text = *textIt;
    text->update();

    textIt = nextTextIt;
  }
}

void TextList::draw (void)
{
  for (auto *text : *this)
		text->draw();
}
