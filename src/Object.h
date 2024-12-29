#pragma once
#include "Triple.h"

constexpr double DEFAULT_KS = 0.7;

class Object
{
protected:
    Point *location;
    double d; // General attributes
    Color *color;
    short state; // State of the object- 0-Normal, 1- Reflective, 2-Transparent
    Color *Ks;
    short shininess; // Material properties

public:
    Object(double _a, double _b, double _c, double _d, double _state);
    Object(const Object &other);            // copy constructor
    Object &operator=(const Object &other); // copy assignment operator
    virtual ~Object();
    Point &getLocation() const;
    Color &getColor() const;
    short getState() const;
    Color &getKs() const;
    short getShininess() const;
    double getD() const;
    void setColor(double _R, double _G, double _B, double _shininess);
    virtual Object *clone() const = 0;
};

class Sphere : public Object
{

public:
    Sphere(double _a, double _b, double _c, double _d, double _state);
    Sphere(const Sphere &other);            // copy constructor
    Sphere &operator=(const Sphere &other); // copy assignment operator
    ~Sphere();
    double getRadius() const;
    Sphere *clone() const override;
};

class Plane : public Object
{

public:
    Plane(double _a, double _b, double _c, double _d, double _state);
    Plane(const Plane &other);            // copy constructor
    Plane &operator=(const Plane &other); // copy assignment operator
    ~Plane();
    Plane *clone() const override;
};