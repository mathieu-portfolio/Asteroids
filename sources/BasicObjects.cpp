#include "GameObjects.h"
#include "GameClock.h"
#include "GameData.h"
#include <iostream>


RenderedObject::RenderedObject(
    float           radius,
    unsigned int    pointCount,
    const Vector2f &position,
    const Color    &baseColor,
    const Time     &twinklePeriod
):
    shape       (new CircleShape(radius,pointCount)),
    baseColor   (baseColor),
    destroyed   (false)
{
    shape->setPosition(position);
    shape->setOrigin(Vector2f(radius,radius));
    shape->setFillColor(baseColor);
    twinkleClock = PeriodicClock(twinklePeriod);
    GameObjects::objects().add(this);
}

RenderedObject::RenderedObject(
    unsigned int    pointCount,
    const Vector2f &position,
    const Color    &baseColor,
    const Time     &twinklePeriod
):
    shape       (new ConvexShape(pointCount)),
    baseColor   (baseColor),
    destroyed   (false)
{
    shape->setPosition(position);
    shape->setFillColor(baseColor);
    twinkleClock = PeriodicClock(twinklePeriod);
    GameObjects::objects().add(this);
}

RenderedObject::RenderedObject(
    const Vector2f &size,
    const Vector2f &position,
    const Color    &baseColor,
    const Time     &twinklePeriod
):
    shape       (new RectangleShape(size)),
    baseColor   (baseColor),
    destroyed   (false)
{
    shape->setPosition(position);
    shape->setOrigin(0.5f * size);
    shape->setFillColor(baseColor);
    twinkleClock = PeriodicClock(twinklePeriod);
    GameObjects::objects().add(this);
}

RenderedObject::~RenderedObject(void)
{
    GameObjects::objects().remove(this);
}


void RenderedObject::twinkle (
    float         delta
)
{
    auto twinkleTime   = twinkleClock.getPeriodicTime().asSeconds();
    auto twinklePeriod = twinkleClock.getPeriod().asSeconds();
    auto currentDelta  = delta * twinkleTime / (0.5f * twinklePeriod);
    if(twinkleClock.isOn())
        currentDelta += -0.5f * delta;
    else
        currentDelta -= delta;

    Color fillColor(
        max(0,min((int)(baseColor.r + currentDelta),255)),
        max(0,min((int)(baseColor.g + currentDelta),255)),
        max(0,min((int)(baseColor.b + currentDelta),255))
    );

    shape->setFillColor(fillColor);
}

void RenderedObject::twinkle(
    const Color  &onColor,
    const Color  &offColor
)
{
    auto fillColor = twinkleClock.getValue<Color>(
        onColor,
        offColor
        );

    shape->setFillColor(fillColor);
}

void RenderedObject::draw (int performances)
{
    auto &window = GameWindow::window();
    window.draw(*shape);
}


MovingObject::MovingObject(
    float			      radius,
    unsigned int	  pointCount,
    const Vector2f &position,
    const Vector2f &direction,
    float			      speed,
    float			      rotationSpeed,
    const Color    &baseColor,
    const Time     &twinklePeriod
):
    RenderedObject    (radius,pointCount,position,baseColor,twinklePeriod),
    direction		      (direction),
    speed			        (speed),
    rotationSpeed     (rotationSpeed)
{
}

MovingObject::MovingObject(
    unsigned int	  pointCount,
    const Vector2f &position,
    const Vector2f &direction,
    float			      speed,
    float			      rotationSpeed,
    const Color    &baseColor,
    const Time     &twinklePeriod
):
    RenderedObject    (pointCount,position,baseColor,twinklePeriod),
    direction		      (direction),
    speed			        (speed),
    rotationSpeed     (rotationSpeed)
{
}

MovingObject::MovingObject(
    const Vector2f &size,
    const Vector2f &position,
    const Vector2f &direction,
    float			      speed,
    float			      rotationSpeed,
    const Color    &baseColor,
    const Time     &twinklePeriod
):
    RenderedObject    (size,position,baseColor,twinklePeriod),
    direction		      (direction),
    speed			        (speed),
    rotationSpeed     (rotationSpeed)
{
}

MovingObject::~MovingObject			(void)
{
}

void MovingObject::move 				(float deltaTime)
{
    shape->move(deltaTime * speed * direction);
#ifdef _WIN32
    shape->rotate(deltaTime * rotationSpeed);
#else
    shape->rotate(degrees(deltaTime * rotationSpeed));
#endif
}

bool MovingObject::outOfBounds	(float radius)
{
    auto windowSize = GameWindow::window().getSize();
    auto position = shape->getPosition() - shape->getOrigin();
    if(position.x + radius < 0             && direction.x < 0)
        return true;
    if(position.x - radius > windowSize.x  && direction.x > 0)
        return true;
    if(position.y + radius < 0             && direction.y < 0)
        return true;
    if(position.y - radius > windowSize.y  && direction.y > 0)
        return true;

    return false;
}

void MovingObject::update   (int performances)
{
    auto deltaTime = GameClock::clock().getDeltaUpdate().asSeconds();
    move(deltaTime);
}


GameObjects *GameObjects::gameObjects = NULL;

GameObjects &GameObjects::objects(void)
{
    if(gameObjects == NULL)
        gameObjects = new GameObjects();
    return *gameObjects;
}

GameObjects::GameObjects(void)
{
}
