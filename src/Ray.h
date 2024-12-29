#pragma once

#include "Triple.h"
#include "Eye.h"
#include <math.h>

class Ray
{
private:
    Point *origin;   // Origin of the ray
    Dvector *vector; // a,b,c of ax+by +cz

public:
    // Constructor
    Ray(const Point &_origin, const Point &point);
    Ray(Point &_origin, Dvector &_vector, double scalar);
    Ray(const Ray &other);            // copy constructor
    Ray &operator=(const Ray &other); // copy assignment operator
    ~Ray();
    Point &getOrigin() const;
    Dvector &getVector() const;
};
