#include "Light.h"
#include "Triple.h"

Light::Light(double _dx, double _dy, double _dz) : directionVector(new Dvector(_dx, _dy, _dz)), intensity(new Color()) {}

Light::Light(const Light &other) : directionVector(other.directionVector->clone()), intensity(other.intensity->clone()) {}

Light &Light::operator=(const Light &other)
{
    if (this != &other)
    {
        delete directionVector;
        this->directionVector = other.directionVector->clone();
        delete intensity;
        this->intensity = other.intensity->clone();
    }
    return *this;
}

Light::~Light()
{
    if (directionVector != nullptr)
    {
        delete directionVector;
        directionVector = nullptr;
    }
    if (intensity != nullptr)
    {
        delete intensity;
        intensity = nullptr;
    }
}

Color &Light::getIntensity() const { return *intensity; }

Dvector &Light::getDirectionVector() const { return *directionVector; }

void Light::setIntensity(double _R, double _G, double _B)
{
    Color *temp = intensity;
    intensity = new Color(_R, _G, _B);
    delete (temp);
}

DirectionalLight::DirectionalLight(double _dx, double _dy, double _dz) : Light(_dx, _dy, _dz) {}

DirectionalLight::DirectionalLight(const DirectionalLight &other) : Light(other) {}

DirectionalLight &DirectionalLight::operator=(const DirectionalLight &other)
{
    if (this != &other)
    {
        Light::operator=(other);
    }
    return *this;
}

DirectionalLight::~DirectionalLight() {}

DirectionalLight *DirectionalLight::clone() const
{
    return new DirectionalLight(*this);
}

Spotlight::Spotlight(double _dx, double _dy, double _dz) : Light(_dx, _dy, _dz), position(new Point()), cosAngle(0.0) {}

Spotlight::Spotlight(const Spotlight &other) : Light(other), position(other.position->clone()) {}

Spotlight &Spotlight::operator=(const Spotlight &other)
{
    if (this != &other)
    {
        Light::operator=(other);
        delete position;
        this->position = other.position->clone();
    }
    return *this;
}

void Spotlight::setPositionAndCosAngle(double _px, double _py, double _pz, double _cosAngle)
{
    Point *temp = position;
    position = new Point(_px, _py, _pz);
    cosAngle = _cosAngle;
    delete (temp);
}

Point Spotlight::getPosition() const { return *position; }

Spotlight::~Spotlight()
{
    delete position;
    position = nullptr;
}

Spotlight *Spotlight::clone() const
{
    return new Spotlight(*this);
}

double Spotlight::getcosAngle() const
{
    return cosAngle;
}