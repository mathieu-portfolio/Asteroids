#include <iostream>
#include "Geometry.h"
#include "AsteroidsObjects.h"
#include <cmath>

using namespace Geometry;

#define MIN_SPEED                     1.f
#define MAX_SPEED                     150.f
#define MIN_RADIUS_TO_HAVE_CRATERS    200.f
#define MIN_RADIUS                    60.f
#define MAX_RADIUS                    400.f
#define MIN_CRATER_RADIUS             50.f
#define MAX_CRATER_RADIUS             200.f
#define MIN_SPAWNTIME                 0.3f
#define MAX_SPAWNTIME                 5.f
#define MIN_SPAWNTIME_RADIUS_BASED    0.6f
#define MIN_FRAGMENTS                 2
#define MAX_FRAGMENTS                 20
#define TIME_BEFORE_MAX_SIZE          120.f
#define TIME_BEFORE_MAX_SPAWN_FREQ    180.f
#define MAX_ASTEROIDS                 400


Asteroid::Asteroid(
  const Vector2f &position,
  const Vector2f &direction,
  float           minSpeed,
  float           maxSpeed,
  float           maxRadius,
  float           baseRadius,
  int             craterCount,
  int             fragmentation,
  const Color    &baseColor
) :
  MovingObject(
    (unsigned int) ((5 + 5 * (baseRadius / maxRadius)) 
      * (1 + 0.5f * randPercent())),
    position,
    direction,
    max(minSpeed, maxSpeed * randPercent()),
    80.f  * (2 * randPercent() - 1),
    baseColor
  ),
  fragmentation     (fragmentation),
  radius            (baseRadius),
  minSpeed          (minSpeed),
  maxSpeed          (maxSpeed),
  maxRadius         (maxRadius)
{
  // random convex shape
  auto  pointCount            = (int) shape->getPointCount();
  randConvexShape((ConvexShape *) shape, pointCount, baseRadius);

  // add craters
  auto lastIdx = 0;
  for (int i = 0; i < craterCount; i++) {
    auto currentIdx = rand() % pointCount;
    auto deltaIdx   = currentIdx - lastIdx;
    if (abs(deltaIdx) < 4) {
      auto sign   = (deltaIdx > 0) - (deltaIdx < 0);
      currentIdx += sign * 2;
    }

    auto craterPosition = shape->getPoint(currentIdx);
    addCrater(craterPosition);
    lastIdx = currentIdx;
  }

  // variations of brown color
  auto depth      = 20 * randPercent() - 10;
  auto r          = baseColor.r + 2 * depth;
  auto g          = baseColor.g + 2 * depth;
  auto b          = baseColor.b +     depth;
  auto color      = Color(r, g, b);
  shape->setFillColor(color);

  // add to list
	AsteroidsObjects::objects().getAsteroids().add(this);
}

Asteroid::Asteroid(
  const Vector2f &position,
  float           minSpeed,
  float           maxSpeed,
  float           maxRadius,
  float           baseRadius,
  int             craterCount,
  int             fragmentation,
  const Color    &baseColor
) :
  Asteroid (
    position,
    Vector2fZero,
    minSpeed,
    maxSpeed,
    maxRadius,
    baseRadius,
    craterCount,
    fragmentation,
    baseColor
  )
{
  // set targetPoint
  if (fragmentation <= 0) {
    auto windowSize = GameWindow::window().getSize();
    auto targetPoint = Vector2f(
      (float) (rand() % (int) windowSize.x),
      (float) (rand() % (int) windowSize.y)
    );
    direction = normalize(targetPoint - shape->getPosition());
  }
}

Asteroid::~Asteroid       (void)
{
  holes.clear();
  // remove from list
  AsteroidsObjects::objects().getAsteroids().remove(this);
}

void Asteroid::addCrater  (const Vector2f &craterPosition)
{
  auto &asteroids             = AsteroidsObjects::objects().getAsteroids();
  auto maxRadius              = asteroids.getMaxRadius();
  auto minRadiusToHaveCraters = asteroids.getMinRadiusToHaveCraters();
  auto minCraterRadius        = asteroids.getMinCraterRadius();
  auto maxCraterRadius        = asteroids.getMaxCraterRadius();

  auto baseCraterRadius = affineValue(
    maxRadius,
    minRadiusToHaveCraters,
    maxCraterRadius,
    minCraterRadius,
    radius
  );
  auto craterRadius = max(
    minCraterRadius,
    min(
      minCraterRadius + (baseCraterRadius - minCraterRadius) * randPercent(),
      maxRadius
    )
  );
  auto craterPoints  = affineValue(
    maxCraterRadius,
    minCraterRadius,
    20.f,
    8.f,
    craterRadius
  );
  craterPoints = max(
    8.f,
    min(
      craterPoints,
      0.f
    )
  );
  auto *craterShape = new ConvexShape((int) craterPoints);
  craterShape->setPosition(shape->getPosition());
  randConvexShape(
    (ConvexShape *) craterShape,
    (int) craterPoints,
    craterRadius
  );
  craterShape->setOrigin(craterPosition);
  craterShape->setFillColor(Color::Transparent);

  holes.push_back(craterShape);
}

void Asteroid::move       (float deltaTime)
{
  MovingObject::move(deltaTime);
  for (auto *hole : holes) {
    hole->move(deltaTime * speed * direction);
    #ifdef _WIN32
      hole->rotate(deltaTime * rotationSpeed);
    #else 
      hole->rotate(degrees(deltaTime * rotationSpeed));
    #endif
  }
}

void Asteroid::destroy    (void)
{
  auto &bullets = AsteroidsObjects::objects().getBullets();
  for(auto *bullet : bullets) {
    // asteroid cross bullet
    auto bulletShape  = *(CircleShape *) bullet->getShape();
    auto bulletCenter = bulletShape.getPosition();
    auto bulletRadius = bulletShape.getRadius();
    auto position     = shape->getPosition();
    
    bool destroy     = false;

    // precise if bullet is big and slow
    if (bulletRadius > 20.f && bullet->getSpeed() < 500.f) {
      // first filter
      if(norm(bulletCenter - position) <= bulletRadius + 1.5f * radius) {
        // second filter
        if (isInside(bulletShape))
          destroy = true;
      }
    }
    // take center of bullet as reference otherwise
    else {
      if(bullet->getSpeed() < 500.f) {
        // first filter
        if(norm(bulletCenter - position) <= bulletRadius + 1.5f * radius) {
          // second filter
          if (Geometry::isInside(bulletCenter, *shape))
            destroy = true;
          for (auto *hole : holes)
            if (Geometry::isInside(bulletCenter, *hole))
              destroy = false;
        }
      }
      else {
        // first filter
        if(norm(bulletCenter - position) <= bulletRadius + radius) {
          destroy = true;
          // second filter
          for (auto *hole : holes)
            if (Geometry::isInside(bulletCenter, *hole))
              destroy = false;
        }
      }
    }

    if(destroy) {
      bullet->doDestroy();
      destroyed = true;
      return;
    }
  }
}

bool  Asteroid::isInside    (const Shape &subjectShape)
{
  // iterate over player shape points
  for(auto i = 0; i < (int) subjectShape.getPointCount(); i++) {
    // includes rotation of the object
    auto point = subjectShape.getTransform().transformPoint(subjectShape.getPoint(i));
    // check if point is inside a hole
    bool inHole = false;
    for(auto *hole : holes) {
      if (Geometry::isInside(point, *hole)) {
        inHole = true;
        break;
      }
    }
    // check if point is inside the asteroid
    bool inAsteroid = Geometry::isInside(point, *shape);
        
    // return true if a point is inside the asteroid shape and not inside a hole
    if (inAsteroid && !inHole)
      return true;
  }

  return false;
}

void Asteroid::fragment   (int performances)
{  
  GameData::data().addScore(fragmentation + 1);
  if (performances == 0)
    return;
  if (AsteroidsObjects::objects().size() > MAX_OBJECTS)
    return;

  auto &asteroids       = AsteroidsObjects::objects().getAsteroids();
  auto minRadius        = asteroids.getMinRadius();
  auto maxRadius        = asteroids.getMaxRadius();
  auto minFragments     = asteroids.getMinFragments();
  auto maxFragments     = asteroids.getMaxFragments();

  if (radius > minRadius) {
    auto position = shape->getPosition();

    // asteroid fragments depends of radius
    auto baseFragments   = min(
      maxFragments,
      (int) affineValue(
        maxRadius,
        minRadius,
        (float) maxFragments,
        (float) minFragments,
        radius
      )
    );
    baseFragments = min(maxFragments, max(baseFragments, minFragments));
    auto fragments              = minFragments + (baseFragments - minFragments) * randPercent();
    auto fragmentsRadius        = (float) (radius / pow(fragments, 0.33f));
    auto minRemainingFragments  = sqrt(fragments);
    auto maxRemainingFragments  = fragments;
    int  remainingFragments     = minRemainingFragments + (maxRemainingFragments - minRemainingFragments) * randPercent();
    if (remainingFragments == 1)
      remainingFragments += rand() % 2;

    // create fragments
    for(int i = 0; i < remainingFragments; i++) {
      // random position base on rotation
      auto fragPosition = position + randPercent() * Geometry::rotate(
        (radius - fragmentsRadius) * Vector2fUp,
        360 * randPercent(),
        AngleUnit::Degrees
      );
      auto fragDirection = 0.3f * normalize(fragPosition - position) + 0.7f * direction;

      // create fragment
      auto *frag      = new Asteroid(
        fragPosition,
        fragDirection,
        minSpeed,
        maxSpeed,
        maxRadius,
        fragmentsRadius,
        0,
        fragmentation + 1,
        baseColor
      );
      auto fragSpeed = frag->getSpeed();
      frag->setSpeed(0.3f * fragSpeed + 0.7f * speed);
    }
  }
}

void Asteroid::update     (int performances)
{
  MovingObject::update(performances);
  
  destroy();

  // delete if destroyed or go out of bounds
  if (outOfBounds(radius))
    destroyed = true;
  else if (destroyed)
    fragment(performances);
}

void Asteroid::draw       (int performances)
{
  auto &window = GameWindow::window();

  if (holes.size() > 0 && performances > 0) {
    // Image img;
    auto &renderTexture = window.getRenderTexture();

    renderTexture.clear(Color::Transparent);
    renderTexture.draw(*shape);
    for (auto *hole : holes) {
      renderTexture.draw(*hole, BlendNone);
    }

    auto texture = renderTexture.getTexture();
    Sprite sprite(texture);
    auto windowSize = window.getSize();
    sprite.setTextureRect(IntRect(
      Vector2i(0, windowSize.y),
      Vector2i(windowSize.x, -(int)windowSize.y)
    ));

    window.draw(sprite);
  }
  else
    window.draw(*shape);
}



Asteroids::Asteroids          (void)
{
  reset();
}

void Asteroids::clean         (void)
{
  auto minAsteroidRadius = max(minRadius, 100.f);
  auto asteroidCount     = size();

  for(auto asteroidIt = begin(); asteroidIt != end() && asteroidCount > MAX_ASTEROIDS; ) {
    auto nextAsteroidIt = asteroidIt;
    nextAsteroidIt++;

    auto asteroid = **asteroidIt;
    if (asteroid.getRadius() < minAsteroidRadius) {
      delete *asteroidIt;
      asteroidCount--;
    }

    asteroidIt = nextAsteroidIt;
  }
}

void Asteroids::spawn         (int performances)
{
  if (performances == 0)
    return;

  auto &clock             = GameClock::clock();
  auto  currentUpdate     = clock.getCurrentUpdate();
  auto  timeSinceSpawn    = (currentUpdate - lastSpawn).asSeconds();

  if (timeSinceSpawn > spawnPeriod) {
    // set parameters
    auto timeSinceStart   = (currentUpdate - clock.getStartingTime()).asSeconds();
    auto baseRadius       = affineValue(
      timeBeforeMaxSize,
      0.f,
      maxRadius,
      minRadius,
      timeSinceStart
    );
    auto randRadius       = max(minRadius, min(minRadius + (baseRadius - minRadius) * randPercent(), maxRadius));

    // add craters
    auto craterCount = 0;
    if(baseRadius > minRadiusToHaveCraters) {
      if (size() < MAX_ASTEROIDS)
        while (randPercent() > 0.9f)
          craterCount++;
    }

    // instanciate asteroid
    auto size = 2.f * Vector2f(randRadius, randRadius);
		new  Asteroid(
      spawnOutOfWindow(size),
      minSpeed,
      maxSpeed,
      maxRadius,
      randRadius,
      craterCount
    );

    // change period
    auto baseSpawnTime = affineValue(
      maxRadius,
      minRadius,
      maxSpawnTime,
      minSpawnTimeRadiusBased,
      randRadius
    );
    baseSpawnTime = max(
      minSpawnTime,
      min(
        baseSpawnTime * (1 - 0.5f * timeSinceStart / timeBeforeMaxSpawnFreq),
        maxSpawnTime
    ));
    spawnPeriod = minSpawnTime + (baseSpawnTime - minSpawnTime) * randPercent();
		setLastSpawn();
	}
}

void Asteroids::update        (int performances)
{
  // spawn new asteroids
  if (size() < MAX_ASTEROIDS)
    spawn(performances);

  // update each asteroid
  ObjectList<Asteroid>::update(performances);

  auto &clock = GameClock::clock();
  auto timeSinceStart = (clock.getCurrentUpdate() - clock.getStartingTime()).asSeconds();
  auto maxMinRadius = 100.f;
  auto maxMaxRadius = 500.f;
  minRadius = min(
    maxMinRadius, 
    MIN_RADIUS + (maxMinRadius - MIN_RADIUS) * timeSinceStart / 240.f
  );
  maxRadius = min(
    maxMaxRadius,
    MAX_RADIUS + (maxMaxRadius - MAX_RADIUS) * timeSinceStart / 240.f
  );

  // clean to prevent lag
  clean();
}

void Asteroids::draw(int performances)
{
  ObjectList<Asteroid>::draw(performances);
  cout << "asteroids draw" << endl;
}

void Asteroids::reset (void)
{
  auto difficulty = GameData::data().getDifficulty();

  spawnPeriod                 = 0.5f;

  minSpeed                    = MIN_SPEED * difficulty;
  maxSpeed                    = MAX_SPEED * difficulty;
  minRadius                   = MIN_RADIUS / difficulty;
  maxRadius                   = MAX_RADIUS;
  minRadiusToHaveCraters      = MIN_RADIUS_TO_HAVE_CRATERS;
  minCraterRadius             = MIN_CRATER_RADIUS;
  maxCraterRadius             = MAX_CRATER_RADIUS;
  minFragments                = MIN_FRAGMENTS;
  maxFragments                = MAX_FRAGMENTS;

  minSpawnTime                = MIN_SPAWNTIME / difficulty;
  minSpawnTimeRadiusBased     = MIN_SPAWNTIME_RADIUS_BASED;
  maxSpawnTime                = MAX_SPAWNTIME;
  timeBeforeMaxSize           = TIME_BEFORE_MAX_SIZE;
  timeBeforeMaxSpawnFreq      = TIME_BEFORE_MAX_SPAWN_FREQ;

  auto currentTime  = GameClock::clock().getCurrentUpdate();
  lastSpawn         = currentTime;
}
