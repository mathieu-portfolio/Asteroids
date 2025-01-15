#pragma once

#include <SFML/Graphics.hpp>
#include "GameList.h"
#include "GameClock.h"

using namespace sf;
using namespace std;


class RenderedObject
{
protected:
    Shape          *shape;
    Color           baseColor;
    bool            destroyed;
    PeriodicClock   twinkleClock;

public:
    RenderedObject(
        float				    radius,
        unsigned int	  pointCount,
        const Vector2f &position        = Vector2f(0.f,0.f),
        const Color    &baseColor       = Color::Magenta,
        const Time     &twinklePeriod   = milliseconds(2000)
    );	// CircleShape
    RenderedObject(
        unsigned int	  pointCount,
        const Vector2f &position        = Vector2f(0.f,0.f),
        const Color    &baseColor       = Color::Magenta,
        const Time     &twinklePeriod   = milliseconds(2000)
    );	// ConvexShape
    RenderedObject(
        const Vector2f &size,
        const Vector2f &position        = Vector2f(0.f,0.f),
        const Color    &baseColor       = Color::Magenta,
        const Time     &twinklePeriod   = milliseconds(2000)
    );	// RectangleShape
    virtual ~RenderedObject(void);

public:
    Shape *getShape     (void)                        { return shape; }
    bool   isDestroyed  (void)                        { return destroyed; }
    void   doDestroy    (bool destroy = true)         { destroyed = destroy; }

protected:
    void   twinkle      (
        const Color  &onColor,
        const Color  &offColor
    );
    void   twinkle      (
        float         delta = 100.f
    );

public:
    virtual void update (int performances) = 0;
    virtual void draw   (int performances);
};

class MovingObject: public RenderedObject
{
protected:
    Vector2f direction;
    float    speed;
    float    rotationSpeed;

public:
    MovingObject(
        float			      radius,
        unsigned int	  pointCount,
        const Vector2f &position,
        const Vector2f &direction,
        float           speed,
        float           rotationSpeed,
        const Color    &baseColor       = Color::Magenta,
        const Time     &twinklePeriod   = milliseconds(2000)
    );	// CircleShape
    MovingObject(
        unsigned int	  pointCount,
        const Vector2f &position,
        const Vector2f &direction,
        float           speed,
        float           rotationSpeed,
        const Color    &baseColor       = Color::Magenta,
        const Time     &twinklePeriod   = milliseconds(2000)
    );	// ConvexShape
    MovingObject(
        const Vector2f &size,
        const Vector2f &position,
        const Vector2f &direction,
        float           speed,
        float           rotationSpeed,
        const Color    &baseColor       = Color::Magenta,
        const Time     &twinklePeriod   = milliseconds(2000)
    );	// RectangleShape
    virtual ~MovingObject(void);

protected:
    virtual void move	       (float deltaTime) = 0;
    virtual bool outOfBounds (float radius);

public:
    Vector2f getDirection     (void)                        { return direction; }
    void     setDirection     (const Vector2f newDirection) { direction = newDirection; }
    float    getSpeed         (void)                        { return speed; }
    void     setSpeed         (float newSpeed)              { speed = newSpeed; }
    float    getRotationSpeed (void)                        { return rotationSpeed; }
    void     setRotationSpeed (float newRotationSpeed)      { rotationSpeed = newRotationSpeed; }

public:
    void update (int performances);
};


template<class T>
class ObjectList: public GameList<T>
{
public:
    ObjectList (void)
    {
    };
    virtual ~ObjectList(void){};

public:
    virtual void update     (int performances)
    {
        for(auto elt = GameList<T>::begin(); elt != GameList<T>::end(); ) {
            auto nextElt = elt;
            nextElt++;
            auto *object = (RenderedObject *)*elt;
            object->update(performances);
            if(object->isDestroyed())
                delete *elt;

            elt = nextElt;
        }
    }

    virtual void draw       (int performances)
    {
        for(auto *elt : *this)
            ((RenderedObject *)elt)->draw(performances);
    }

    virtual void reset(void){};
};


class GameObjects: public ObjectList<RenderedObject>
{
protected:

public:
    static GameObjects *gameObjects;

    static GameObjects &objects    (void);
    GameObjects  (void);
};

