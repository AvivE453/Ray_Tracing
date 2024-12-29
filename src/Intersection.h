#pragma once

#include "Triple.h"
#include "Object.h"

class Intersection
{
private:
    Object *object; // a,b,c of ax+by +cz
    Point *hit;     // Origin of the ray
    double dist;

public:
    // Constructor
    Intersection(Object &primitive, Point &hit, double dist);
    ~Intersection();

    // Getters for origin and direction
    Object &getObject() const;
    Point &getHit() const;
    double getDist() const;

    Intersection *clone() const;
};
