#include "Geometry.h"
#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>
using namespace std;

using namespace Geometry;

Vector2f			    Geometry::rotate(
  const Vector2f &v,
  float angle,
  AngleUnit unit
)
{
	switch(unit) {
	case (AngleUnit::Radians):

		break;
	case (AngleUnit::Degrees):
		angle =  M_PI / 180 * angle;
		break;
	}

	auto cos_val = cos(angle);
	auto sin_val = sin(angle);

	return Vector2f( cos_val * v.x - sin_val * v.y, sin_val * v.x + cos_val * v.y );
}

float             Geometry::angle(
  const Vector2f &a,
  const Vector2f &b,
  AngleUnit unit
)
{
  float res = atan2f(b.y, b.x) - atan2f(a.y, a.x);

  switch(unit) {
		case(AngleUnit::Radians):
			return res;

		case(AngleUnit::Degrees):
			return res / M_PI * 180;

		default:
			return res;
  }

	return res;
}


float				      Geometry::norm		    (const	Vector2f &v)
{
	return sqrt(v.x * v.x + v.y * v.y);
}

Vector2f			    Geometry::normalize   (const	Vector2f &v)
{
	auto norme = Geometry::norm(v);

	if(norme == 0) {
		return v;
	}

	return v / norme;
}

ostream			     &Geometry::operator << (
  ostream &os,
  const Vector2f &v
) 
{ 
  os << "(" << v.x << "," << v.y << ")";
  return os;
}

const float			  Geometry::dot(
	const Vector2f &a,
	const Vector2f &b
)
{ 
	return a.x * b.x + a.y * b.y; 
}

const float       Geometry::cross(
	const Vector2f &a,
  const Vector2f &b
)
{ 
	return a.x * b.y - b.x * a.y;
}
const bool			  Geometry::isInside(
	const Vector2f &point,
	const Vector2f &a,
	const Vector2f &b
)
{
	return (cross(a - b, point) + cross(b, a)) < 0.0f;
}

const bool			  Geometry::isInside(
	const Vector2f &point,
	const Shape    &shape
)
{
  auto shapeSize = (int) shape.getPointCount();
  if (shapeSize < 3)
    return false;

  Vector2f shapePos(shape.getPosition());
  for (int shapeIdx = 0; shapeIdx < shapeSize; shapeIdx++) {
    Vector2f a(shape.getTransform().transformPoint(shape.getPoint(shapeIdx)));
    Vector2f b(shape.getTransform().transformPoint(shape.getPoint((shapeIdx + 1) % shapeSize)));

    if (!isInside(point, a, b))
      return false;
  }

	return true;
}

/* Check if a point of subjectShape is inside clipShape */
const bool			  Geometry::isInside(
	const Shape    &subjectShape,
	const Shape    &clipShape,
  bool            partially
)
{
  auto subjectSize = (int) subjectShape.getPointCount();
  if (subjectSize < 1)
    return false;

  Vector2f subjectPos(subjectShape.getPosition());
  for (int subjectIdx = 0; subjectIdx < subjectSize; subjectIdx++) {
    Vector2f p(subjectShape.getTransform().transformPoint(subjectShape.getPoint(subjectIdx)));
    if (isInside(p, clipShape)) {
      if (partially)
        return true;
		}
    else if (!partially)
      return false;
  }

	return !partially;
}

const Vector2f		Geometry::intersection(
	const Vector2f &a1,
	const Vector2f &a2,
	const Vector2f &b1,
	const Vector2f &b2
)
{
	return ((b1 - b2) * cross(a1, a2) - (a1 - a2) * cross(b1, b2)) * (1.0f / cross(a1 - a2, b1 - b2));
}

float sqr(float val) { return val * val; }

const bool Geometry::intersectSphere(
  const Vector2f &a1,
	const Vector2f &a2,
	const Vector2f &s,
	float radius
)
{
  if (dot(a2 - a1, s - a1) < 0)
    return false;

  auto d2 = norm(a2 - a1);
  auto ds = norm(s - a1);
  if (d2 < ds - radius)
    return false;

  auto a = sqr(a2.x - a1.x) + sqr(a2.y - a1.y);
  auto b = 2 * ((a2.x - a1.x) * (a1.x - s.x) + (a2.y - a1.y) * (a1.y - s.y));
  auto c = sqr(s.x) + sqr(s.y) + sqr(a1.x) + sqr(a1.y) - 2 * (s.x * a1.x + s.y * a1.y) - sqr(radius);

  return sqr(b) - 4 * a * c >= 0;
}

vector<Vector2f>	Geometry::sutherLandHodgman(
  const Shape &subjectPolygon, 
	const Shape &clipPolygon
)
{
  auto clipSize    = (int) clipPolygon.getPointCount();
  auto subjectSize = (int) subjectPolygon.getPointCount();

	if (clipSize == 0 || subjectSize == 0)
		return {};

	vector<Vector2f> ring;
  for (int subjectIdx = 0; subjectIdx < subjectSize; subjectIdx++)
    ring.push_back(subjectPolygon.getTransform().transformPoint(subjectPolygon.getPoint(subjectIdx)));

	Vector2f p1 = clipPolygon.getTransform().transformPoint(clipPolygon.getPoint(clipSize - 1));

	vector<Vector2f> input;

	for (int clipIdx = 0; clipIdx < clipSize; clipIdx++) {
    auto p2 = clipPolygon.getTransform().transformPoint(clipPolygon.getPoint(clipIdx));
		input.clear();
		input.insert(input.end(), ring.begin(), ring.end());
		auto s = input[input.size() - 1];

		ring.clear();

		for (const auto &e : input) {
			if (isInside(e, p1, p2)) {
				if (!isInside(s, p1, p2))
					ring.push_back(intersection(p1, p2, s, e));

				ring.push_back(e);
			} 
      else if (isInside(s, p1, p2))
				ring.push_back(intersection(p1, p2, s, e));

			s = e;
		}

		p1 = p2;
	}

	return ring;
}

void Geometry::randConvexShape(
  ConvexShape *shape,
  int pointCount,
  float baseRadius
)
{
  auto anglePortion  = (float) 360 / pointCount;
  for (auto i = 0; i < pointCount; i++) {
    auto randAngle   = rand() % (int) anglePortion;
    auto dir         = rotate(
      Vector2f(1, 0),
      i * anglePortion + randAngle,
      AngleUnit::Degrees
    );
    auto point       = baseRadius * dir;
    shape->setPoint(i, point);
	}
}

float Geometry::affineValue   (float xmax, float xmin, float ymax, float ymin, float x)
{
  return ymin + (x - xmin) / (xmax - xmin) * ymax;
}

float Geometry::randPercent   (void)
{
  return 0.01f * (rand() % 101);
}

