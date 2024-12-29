#pragma once

#include <math.h>

class Triple
{
protected:
	double first, second, third;

public: // no need for copy constructor and copy assignment operator all fild are regular type
	Triple();
	Triple(double first, double second, double third);
	double getFirst() const;
	double getSecond() const;
	double getThird() const;
	virtual Triple *clone() const = 0;
	virtual ~Triple() = default; // deafult destructor (we dont have pointers)
};

class Point : public Triple
{

public:
	Point();
	Point(double x, double y, double z);
	Point *clone() const override;
	~Point();
};

class Color : public Triple
{

public:
	Color();
	Color(double R, double G, double B);
	Color *clone() const override;
	void add(const Color &other);
	void scale(double scalar);
	void mult(const Color &other);
	void clip();
	~Color();
};

class Dvector : public Triple
{

public:
	Dvector();
	Dvector(const Point &origin, const Point &point);
	Dvector(double x, double y, double z);
	Dvector *clone() const override;
	void add(const Dvector &other);
	void scale(double scalar);
	void mult(const Dvector &other);
	void normalize();
	~Dvector();
};
