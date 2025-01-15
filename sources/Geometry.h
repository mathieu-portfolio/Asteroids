#pragma once

#include <SFML/Graphics.hpp>
#include <ostream>

using namespace sf;
using namespace std;

namespace Geometry {
  const Vector2f Vector2fOne  ( 1.f,  1.f);
  const Vector2f Vector2fZero ( 0.f,  0.f);
  const Vector2f Vector2fUp   ( 0.f, -1.f);
  const Vector2f Vector2fDown ( 0.f,  1.f);
  const Vector2f Vector2fLeft (-1.f,  0.f);
  const Vector2f Vector2fRight( 1.f,  0.f);
	
	enum class AngleUnit { Radians, Degrees };


	Vector2f          rotate(
    const	Vector2f &v,
    float angle,
    AngleUnit unit = AngleUnit::Radians
  );
  float             angle(
    const Vector2f &a,
    const Vector2f &b,
    AngleUnit unit = AngleUnit::Radians
  );
	float		          norm		    (const	Vector2f &v);
	Vector2f          normalize   (const	Vector2f &v);

	ostream          &operator << (
    ostream &os,
    const Vector2f &v
  );

	const float       dot(
    const Vector2f &a,
    const Vector2f &b
  );
	const float       cross(
    const Vector2f &a,
    const Vector2f &b
  );

  // check if a point is on the LEFT side of an edge
	const bool        isInside(
		const Vector2f &point,
		const Vector2f &a,
		const Vector2f &b
	);

  const bool        isInside(
		const Vector2f &point,
		const Shape    &shape
	);
  
  const bool        isInside(
		const Shape    &subjectShape,
		const Shape    &clipShape,
    bool            partially = true
	);

  // calculate intersection point
	const Vector2f    intersection(
		const Vector2f &a1,
		const Vector2f &a2,
		const Vector2f &b1,
		const Vector2f &b2
	);

  // check if shpere is in trajectory
	const bool intersectSphere(
		const Vector2f &a1,
		const Vector2f &a2,
		const Vector2f &s,
		float radius
	);

	// Sutherland-Hodgman clipping
	vector<Vector2f>  sutherLandHodgman(
		const Shape &subjectPolygon, 
		const Shape &clipPolygon
	);

  // create a randomized convex shape
  void randConvexShape(
    ConvexShape    *shape,
    int             pointCount,
    float           baseRadius
  );

  float affineValue   (float xmax, float xmin, float ymax, float ymin, float x);

  float randPercent   (void);
}


