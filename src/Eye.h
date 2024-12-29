#pragma once
#include "Triple.h"

class Eye
{
private:
    Point *location; // Location of the eye

public:
    Eye();
    Eye(double _x, double _y, double _z);
    Eye(const Eye &other);            // copy constructor
    Eye &operator=(const Eye &other); // copy assignment operator
    ~Eye();
    Eye *clone() const;

    // Getter for location
    Point &getLocation();
};
