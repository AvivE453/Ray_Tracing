#include "Object.h"

Object::Object(double _a, double _b, double _c, double _d, double _state) : d(_d), state(_state)
{
  location = new Point(_a, _b, _c);
  Ks = new Color(DEFAULT_KS, DEFAULT_KS, DEFAULT_KS);
  color = new Color();
}

Object::Object(const Object &other) : location(other.location->clone()), d(other.d), color(other.color->clone()),
                                      state(other.state), Ks(other.Ks->clone()), shininess(other.shininess)
{
}

Object &Object::operator=(const Object &other)
{
  if (this != &other)
  {
    this->d = other.d;
    this->state = other.state;
    this->shininess = other.shininess;
    delete location;
    this->location = other.location->clone();
    delete Ks;
    this->Ks = other.Ks->clone();
    delete color;
    this->color = other.color->clone();
  }
  return *this;
}

// Getter implementations
Point &Object::getLocation() const { return *location; }
Color &Object::getColor() const { return *color; }
short Object::getState() const { return state; }
Color &Object::getKs() const { return *Ks; }
short Object::getShininess() const { return shininess; }
double Object::getD() const { return d; }

void Object::setColor(double _R, double _G, double _B, double _shininess)
{
  Color *temp = color;
  color = new Color(_R, _G, _B);
  shininess = _shininess;
  delete (temp);
}

Object::~Object()
{
  if (color != nullptr)
  {
    delete color;
    color = nullptr;
  }
  if (Ks != nullptr)
  {
    delete Ks;
    Ks = nullptr;
  }
  if (location != nullptr)
  {
    delete location;
    location = nullptr;
  }
}

Sphere::Sphere(double _a, double _b, double _c, double _d, double _state) : Object(_a, _b, _c, _d, _state) {}

double Sphere::getRadius() const { return d; }

Sphere::~Sphere() {}

Sphere *Sphere::clone() const
{
  return new Sphere(*this);
}

Sphere::Sphere(const Sphere &other) : Object(other) {}

Sphere &Sphere::operator=(const Sphere &other)
{
  if (this != &other)
  {
    Object::operator=(other);
  }
  return *this;
}

Plane::Plane(double _a, double _b, double _c, double _d, double _state) : Object(_a, _b, _c, _d, _state) {}

Plane::~Plane() {}

Plane *Plane::clone() const
{
  return new Plane(*this);
}

Plane::Plane(const Plane &other) : Object(other) {}

Plane &Plane::operator=(const Plane &other)
{
  if (this != &other)
  {
    Object::operator=(other);
  }
  return *this;
}
