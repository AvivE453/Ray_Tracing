#include "Eye.h"

Eye::Eye() : location(new Point()) {}

Eye::Eye(double _x, double _y, double _z) : location(new Point(_x, _y, _z)) {}

Eye::Eye(const Eye &other) : location(other.location->clone()) {}

Eye &Eye::operator=(const Eye &other)
{
    if (this != &other)
    {
        delete location;
        this->location = other.location->clone();
    }
    return *this;
}

Eye::~Eye()
{
    if (location != nullptr)
    {
        delete location;
        location = nullptr;
    }
}

Eye *Eye::clone() const
{

    return new Eye(*this);
}

Point &Eye::getLocation() { return *location; }
