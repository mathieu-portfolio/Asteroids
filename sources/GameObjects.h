#pragma once

#include <SFML/Graphics.hpp>
#include "GameList.h"
#include "GameClock.h"

using namespace sf;
using namespace std;


/**
 * @class RenderedObject
 * @brief Base class for objects rendered in the game.
 */
class RenderedObject
{
protected:
    Shape          *shape;       ///< Shape of the object.
    Color           baseColor;   ///< Base color of the object.
    bool            destroyed;   ///< Indicates if the object is destroyed.
    PeriodicClock   twinkleClock; ///< Clock for twinkle effect.

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

/**
 * @class MovingObject
 * @brief Base class for moving objects in the game.
 */
class MovingObject: public RenderedObject
{
protected:
    Vector2f direction; ///< Direction of movement.
    float speed;        ///< Speed of movement.
    float rotationSpeed; ///< Rotation speed.

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


/**
 * @class ObjectList
 * @brief A templated list for managing objects.
 * @tparam T Type of objects stored in the list.
 */
template<class T>
class ObjectList: public GameList<T>
{
public:
    ObjectList (void)
    {
    };
    virtual ~ObjectList(void){};

public:
    /**
     * @brief Updates all objects in the list.
     * @param performances Performance factor for update timing.
     */
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

    /**
     * @brief Draws all objects in the list.
     * @param performances Performance factor for rendering.
     */
    virtual void draw       (int performances)
    {
        for(auto *elt : *this)
            ((RenderedObject *)elt)->draw(performances);
    }

    virtual void reset(void){};
};


/**
 * @class GameObjects
 * @brief Manages all game objects.
 */
class GameObjects: public ObjectList<RenderedObject>
{
public:
    static GameObjects *gameObjects; ///< Singleton instance.

    /**
     * @brief Gets the singleton instance of GameObjects.
     * @return Reference to the GameObjects instance.
     */
    static GameObjects &objects(void);

    /**
     * @brief Constructs a GameObjects object.
     */
    GameObjects(void);
};
