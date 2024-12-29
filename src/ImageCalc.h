#pragma once

#include "Scene.h"
#include "Image.h"
#include "Ray.h"
#include "Intersection.h"
#include "Triple.h"
#include "Light.h"
#include "Object.h"
#include <iostream>
#include <algorithm>
#include <cfloat>	 // For DBL_MAX
#include <cmath>	 // For math functions like sqrt, pow
#include <algorithm> // For std::min
#include <climits>

const int MAX_LEVEL = 5;
const double ref_index = 1.5;
const double air_ref_index = 1.0;

// Ray casting function
class ImageCalc
{
public:
	static void RayCast(Scene &scene, Image &image);

	// Intersection-related functions
	static Intersection *FindIntersection(const Ray &ray, Scene &scene);
	static Intersection *FindIntersectionWithOthers(const Ray &ray, Scene &scene, Object &self);

	static double Intersect(const Ray &ray, const Object &primitive);

	// Normal calculation
	static Dvector *calcNormal(Intersection &inter);

	// Color calculation functions
	static Color *CalcColor(Scene &scene, const Ray &in_ray, int level);
	static Color *calcDiffuseColor(const Ray &in_ray, Intersection &inter, Light &light);
	static Color *calcSpecularColor(const Ray &in_ray, Intersection &inter, Light &light);

	// Utility vector and color operations
	static double dotProduct(Dvector &first, Dvector &second);
	static Dvector *calcReflective(Dvector &normal, Dvector &light_vector);
	static void checkerboardColor(Color &color, Point &hitPoint);
	static Color *calcColorForOpaqueObject(Scene &scene, const Ray &in_ray, Intersection &inter);
	static Color *calcColorForTransparentObject(Scene &scene, const Ray &in_ray, Intersection &inter, int level);
	static Dvector *snell_law(Dvector &normal, Dvector &ray_direction, bool flag);
	static double distance(const Point &first, const Point &second);
};