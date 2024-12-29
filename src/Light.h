#pragma once
#include "Triple.h"

class Light
{

protected:
    Dvector *directionVector; // Direction vector components
    Color *intensity;
    Light(double _dx, double _dy, double _dz);
    Light(const Light &other);            // copy constructor
    Light &operator=(const Light &other); // copy assignment operator

public:
    Color &getIntensity() const;
    Dvector &getDirectionVector() const;
    virtual ~Light();
    void setIntensity(double _R, double _G, double _B);
    virtual Light *clone() const = 0; // Pure virtual function to make Light an abstract class
};

class DirectionalLight : public Light
{

public:
    DirectionalLight(double _dx, double _dy, double _dz);
    DirectionalLight(const DirectionalLight &other);            // copy constructor
    DirectionalLight &operator=(const DirectionalLight &other); // copy assignment operator
    ~DirectionalLight();
    DirectionalLight *clone() const override;
};

class Spotlight : public Light
{
protected:
    Point *position; // Position components
    double cosAngle;

public:
    Spotlight(double _dx, double _dy, double _dz);
    Spotlight(const Spotlight &other);            // copy constructor
    Spotlight &operator=(const Spotlight &other); // copy assignment operator
    ~Spotlight();
    void setPositionAndCosAngle(double _px, double _py, double _pz, double cosAngle);
    Point getPosition() const;
    double getcosAngle() const;
    Spotlight *clone() const override;
};
