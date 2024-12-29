#include "Triple.h"

// Constructor implementation
Triple::Triple() : first(0.0), second(0.0), third(0.0) {}
Triple::Triple(double first, double second, double third) : first(first), second(second), third(third) {}

double Triple::getFirst() const { return first; }
double Triple::getSecond() const { return second; }
double Triple::getThird() const { return third; }

Point::Point() : Triple() {}
Point::Point(double x, double y, double z) : Triple(x, y, z) {}
Point::~Point() {}

Point *Point::clone() const
{
	return new Point(*this);
}

Color::Color() : Triple() {}
Color::Color(double R, double G, double B) : Triple(R, G, B) {}
Color::~Color() {}

void Color::add(const Color &other)
{
	first += other.getFirst();
	second += other.getSecond();
	third += other.getThird();
}

void Color::scale(double scalar)
{
	first = scalar * first;
	second = scalar * second;
	third = scalar * third;
}

void Color::mult(const Color &other)
{
	first = first * other.getFirst();
	second = second * other.getSecond();
	third = third * other.getThird();
}

void Color::clip()
{
	first = std::max(std::min(first, 1.0), 0.0);
	second = std::max(std::min(second, 1.0), 0.0);
	third = std::max(std::min(third, 1.0), 0.0);
}

Color *Color::clone() const
{
	return new Color(*this);
}

Dvector::Dvector() : Triple() {}

Dvector::Dvector(double x, double y, double z)
{
	double dist = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	first = x / dist;
	second = y / dist;
	third = z / dist;
}

Dvector::Dvector(const Point &origin, const Point &point)
{
	double x = point.getFirst() - origin.getFirst();
	double y = point.getSecond() - origin.getSecond();
	double z = point.getThird() - origin.getThird();
	double dist = sqrt(x * x + y * y + z * z);
	first = x / dist;
	second = y / dist;
	third = z / dist;
}

void Dvector::add(const Dvector &other)
{
	first += other.getFirst();
	second += other.getSecond();
	third += other.getThird();
}

void Dvector::scale(double scalar)
{
	first = scalar * first;
	second = scalar * second;
	third = scalar * third;
}

void Dvector::mult(const Dvector &other)
{
	first = first * other.getFirst();
	second = second * other.getSecond();
	third = third * other.getThird();
}

void Dvector::normalize()
{
	double dist = sqrt(pow(first, 2) + pow(second, 2) + pow(third, 2));
	first = first / dist;
	second = second / dist;
	third = third / dist;
}

Dvector::~Dvector() {}

Dvector *Dvector::clone() const
{
	return new Dvector(*this);
}
