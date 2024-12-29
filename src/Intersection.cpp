#include "Intersection.h"

Intersection::Intersection(Object &_object, Point &_hit, double _dist) : object(&_object), hit(&_hit), dist(_dist) {}

Intersection::~Intersection()
{
}

Object &Intersection::getObject() const
{
    return *object;
}
Point &Intersection::getHit() const
{
    return *hit;
}
double Intersection::getDist() const
{
    return dist;
}

Intersection *Intersection::clone() const
{
    return new Intersection(*this);
}