#include "Ray.h"
#include "Triple.h"
using namespace std;

// Constructor implementation
Ray::Ray(const Point &_origin, const Point &_point)
{
        origin = new Point(_origin);
        vector = new Dvector(_origin, _point);
}

Ray::Ray(Point &_origin, Dvector &_vector, double scalar) // copies the origin and the dvector, and scales the dvector in scalar
{
        origin = new Point(_origin);
        vector = new Dvector(_vector);
        vector->scale(scalar);
        vector->normalize();
}
Ray::Ray(const Ray &other) : origin(other.origin->clone()), vector(other.vector->clone()) {}

Ray &Ray::operator=(const Ray &other)
{
        if (this != &other)
        {
                delete origin;
                this->origin = other.origin->clone();
                delete vector;
                this->vector = other.vector->clone();
        }
        return *this;
}

Point &Ray::getOrigin() const
{
        return *origin;
}

Dvector &Ray::getVector() const
{
        return *vector;
}

Ray::~Ray()
{
        if (origin != nullptr)
        {
                delete origin;
                origin = nullptr;
        }
        if (vector != nullptr)
        {
                delete vector;
                vector = nullptr;
        }
}
