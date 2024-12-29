#include "ImageCalc.h"
using namespace std;

void ImageCalc::RayCast(Scene &scene, Image &image)
{
	int width = image.getWidth();
	int height = image.getHeight();
	int comps = image.getComps(); // This should be 4 (RGBA)
	unsigned char *buffer = image.getBuffer();

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			// Convert the pixel (i, j) to the normalized device coordinates
			double x = (2 * j / (double)width) - 1;
			double y = 1 - (2 * i / (double)height);
			double z = 0;
			Point *p = new Point(x, y, z);

			// Create a ray from the eye to the point (p)
			Ray *ray = new Ray(scene.getEye().getLocation(), *p);

			// Calculate the color for the pixel
			Color *color = CalcColor(scene, *ray, 0);

			// Extract the RGB components of the color
			double r = color->getFirst();
			double g = color->getSecond();
			double b = color->getThird();

			// Calculate the buffer index for the current pixel
			int pixelIndex = (i * width + j) * comps;

			// Assign color values to the RGBA channels in the buffer
			buffer[pixelIndex + 0] = (unsigned char)(255 * r); // Red
			buffer[pixelIndex + 1] = (unsigned char)(255 * g); // Green
			buffer[pixelIndex + 2] = (unsigned char)(255 * b); // Blue
			buffer[pixelIndex + 3] = 255;					   // Alpha (fully opaque)

			// Clean up dynamically allocated objects
			delete color;
			delete ray;
			delete p;
		}
	}
}

Color *ImageCalc::CalcColor(Scene &scene, const Ray &in_ray, int level)
{

	Color *final_color = new Color(0.0, 0.0, 0.0);
	if (level < MAX_LEVEL)
	{
		Intersection *inter = FindIntersection(in_ray, scene);
		if (inter != nullptr)
		{
			if (inter->getObject().getState() == 0) // the object is opaque
			{
				Color *opaqueColor = calcColorForOpaqueObject(scene, in_ray, *inter);
				final_color->add(*opaqueColor);
				delete opaqueColor;
			}
			else if (inter->getObject().getState() == 1) // the object is reflective
			{
				Dvector *normal = calcNormal(*inter);
				Dvector *reflective_vector = calcReflective(*normal, in_ray.getVector());
				Ray *reflective_ray = new Ray(inter->getHit(), *reflective_vector, 1.0);
				Color *reflectiveColor = CalcColor(scene, *reflective_ray, level + 1);
				final_color->add(*reflectiveColor);
				delete normal;
				delete reflective_vector;
				delete reflective_ray;
				delete reflectiveColor;
			}
			else // the object is transparent
			{
				Color *transparentColor = calcColorForTransparentObject(scene, in_ray, *inter, level);
				final_color->add(*transparentColor);
				delete transparentColor;
			}
			delete &inter->getHit(); // new
			delete inter;			 // new
		}
	}

	final_color->clip();
	return final_color;
}

Color *ImageCalc::calcColorForTransparentObject(Scene &scene, const Ray &in_ray, Intersection &inter, int level)
{
	Color *final_color = new Color(0.0, 0.0, 0.0);
	Dvector *normal = calcNormal(inter);
	Dvector *transparent_vector;
	Ray *transparent_ray;
	Intersection *out_inter;
	Dvector *out_normal;
	Dvector *out_vector;
	Ray *out_ray;
	Color *recursive;

	transparent_vector = snell_law(*normal, in_ray.getVector(), true);
	transparent_ray = new Ray(inter.getHit(), *transparent_vector, 1.0);
	out_inter = FindIntersection(*transparent_ray, scene);
	out_normal = calcNormal(*out_inter);
	out_normal->scale(-1); // we want the opposite for the normal cause we need to calaulate with the regular normal
	out_vector = snell_law(*out_normal, *transparent_vector, false);
	out_ray = new Ray(out_inter->getHit(), *out_vector, 1.0);
	recursive = CalcColor(scene, *out_ray, level + 1);
	final_color->add(*recursive);

	delete transparent_vector;
	delete transparent_ray;
	delete &out_inter->getHit(); // new
	delete out_inter;
	delete out_normal;
	delete out_vector;
	delete out_ray;
	delete recursive;

	return final_color;
}

Dvector *ImageCalc::snell_law(Dvector &normal, Dvector &ray_direction, bool flag)
{
	double ref_ratio = ref_index / air_ref_index;
	if (flag)
	{
		ref_ratio = air_ref_index / ref_index;
	}
	Dvector *ray_copy = new Dvector(ray_direction);
	ray_copy->scale(ref_ratio);
	normal.scale(-1); // to do dot product with the opposite vector of the normal
	double cosTheta_i = dotProduct(normal, ray_direction);
	double cosTheta_r = cos(asin((sin(acos(cosTheta_i)) * ref_ratio))); // asin((sin(acos(cosTheta_i)) * ref_ratio)) = Theta_r
	double scalar = ref_ratio * cosTheta_i - cosTheta_r;
	normal.scale(-1); // turn normal to the first vector again
	Dvector *transparent_ray = new Dvector(normal);
	transparent_ray->scale(scalar);
	transparent_ray->add(*ray_copy);
	delete ray_copy;
	return transparent_ray;
}

Color *ImageCalc::calcColorForOpaqueObject(Scene &scene, const Ray &in_ray, Intersection &inter)
{
	Color *final_color = new Color(0.0, 0.0, 0.0);
	final_color->add(scene.getAmbient());
	final_color->mult(inter.getObject().getColor());
	Light *light;
	Ray *opposite_ray;
	Intersection *shadowed;
	Color *Diffuse_color;
	Color *Specular_color;
	for (size_t i = 0; i < scene.getLightsSize(); i++)
	{
		light = &(scene.getNextDirectionalLight());
		opposite_ray = new Ray(inter.getHit(), light->getDirectionVector(), -1.0);
		shadowed = FindIntersectionWithOthers(*opposite_ray, scene, inter.getObject());
		delete opposite_ray;
		if (!shadowed)
		{
			Diffuse_color = calcDiffuseColor(in_ray, inter, *light);
			Specular_color = calcSpecularColor(in_ray, inter, *light);
			final_color->add(*Diffuse_color);
			final_color->add(*Specular_color);
			delete Diffuse_color;
			delete Specular_color;
		}
		else
		{
			delete &shadowed->getHit(); // new
			delete shadowed;
		}
	}
	for (size_t i = 0; i < scene.getSpotlightsSize(); i++)
	{

		light = &(scene.getNextSpotlight());
		Spotlight *spotlight = dynamic_cast<Spotlight *>(light);
		opposite_ray = new Ray(inter.getHit(), spotlight->getPosition());
		double dist = distance(inter.getHit(), spotlight->getPosition());
		shadowed = FindIntersectionWithOthers(*opposite_ray, scene, inter.getObject());
		delete opposite_ray;
		if (!shadowed || dist < shadowed->getDist())
		{
			Diffuse_color = calcDiffuseColor(in_ray, inter, *light);
			Specular_color = calcSpecularColor(in_ray, inter, *light);
			final_color->add(*Diffuse_color);
			final_color->add(*Specular_color);
			delete Diffuse_color;
			delete Specular_color;
		}
		else
		{
			delete &shadowed->getHit(); // new
			delete shadowed;
		}
	}
	if (inter.getObject().getD() <= 0)
	{
		checkerboardColor(*final_color, inter.getHit());
	}
	return final_color;
}

Intersection *ImageCalc::FindIntersection(const Ray &ray, Scene &scene)
{
	double min_t = DBL_MAX;
	Object *min_primitive = NULL;
	double t;
	Object *primitive;
	for (size_t i = 0; i < scene.getObjectsSize(); i++)
	{

		primitive = &(scene.getNextObject());
		t = Intersect(ray, *primitive);
		if (0.01 < t && t < min_t)
		{
			min_primitive = primitive;
			min_t = t;
		}
	}
	if (min_t == DBL_MAX)
	{
		return nullptr;
	}
	double origin_x = ray.getOrigin().getFirst();
	double origin_y = ray.getOrigin().getSecond();
	double origin_z = ray.getOrigin().getThird();
	double x = ray.getVector().getFirst();
	double y = ray.getVector().getSecond();
	double z = ray.getVector().getThird();
	double x_hit = origin_x + min_t * x;
	double y_hit = origin_y + min_t * y;
	double z_hit = origin_z + min_t * z;
	Point *hit = new Point(x_hit, y_hit, z_hit);
	return new Intersection(*min_primitive, *hit, min_t);
}

Intersection *ImageCalc::FindIntersectionWithOthers(const Ray &ray, Scene &scene, Object &self)
{
	double min_t = DBL_MAX;
	Object *min_primitive = NULL;
	double t;
	Object *primitive;
	for (size_t i = 0; i < scene.getObjectsSize(); i++)
	{
		primitive = &(scene.getNextObject());
		t = Intersect(ray, *primitive);
		if (t > 0.01 && t < min_t && primitive != &self) // t>0 because the plane is under the spheres so we want it to show the spheres
		{
			min_primitive = primitive;
			min_t = t;
		}
	}
	if (min_t == DBL_MAX)
	{
		return nullptr;
	}
	double origin_x = ray.getOrigin().getFirst();
	double origin_y = ray.getOrigin().getSecond();
	double origin_z = ray.getOrigin().getThird();
	double x = ray.getVector().getFirst();
	double y = ray.getVector().getSecond();
	double z = ray.getVector().getThird();
	double x_hit = origin_x + min_t * x;
	double y_hit = origin_y + min_t * y;
	double z_hit = origin_z + min_t * z;
	Point *hit = new Point(x_hit, y_hit, z_hit);
	return new Intersection(*min_primitive, *hit, min_t);
}

double ImageCalc::Intersect(const Ray &ray, const Object &primitive)
{
	double t = DBL_MAX;
	if (primitive.getD() <= 0) // the object is plane
	{
		double a = primitive.getLocation().getFirst();
		double b = primitive.getLocation().getSecond();
		double c = primitive.getLocation().getThird();
		double d = primitive.getD();
		double x_origin = ray.getOrigin().getFirst();
		double y_origin = ray.getOrigin().getSecond();
		double z_origin = ray.getOrigin().getThird();
		double x = ray.getVector().getFirst();
		double y = ray.getVector().getSecond();
		double z = ray.getVector().getThird();
		double divider = a * x + b * y + c * z; // to check if the vector & plane are parallal
		if (divider != 0)
		{
			t = -(a * x_origin + b * y_origin + c * z_origin + d) / divider;
		}
	}
	else
	{ // the object is sphere
		double x_sphere = primitive.getLocation().getFirst();
		double y_sphere = primitive.getLocation().getSecond();
		double z_sphere = primitive.getLocation().getThird();
		double r = primitive.getD();
		double x_origin = ray.getOrigin().getFirst();
		double y_origin = ray.getOrigin().getSecond();
		double z_origin = ray.getOrigin().getThird();
		double x = ray.getVector().getFirst();
		double y = ray.getVector().getSecond();
		double z = ray.getVector().getThird();

		double dx = x_origin - x_sphere;
		double dy = y_origin - y_sphere;
		double dz = z_origin - z_sphere;

		double A = pow(x, 2) + pow(y, 2) + pow(z, 2);
		double B = 2 * (x * dx + y * dy + z * dz);
		double C = pow(dx, 2) + pow(dy, 2) + pow(dz, 2) - pow(r, 2);
		double det = B * B - 4 * A * C;
		if (0 <= det)
		{
			double t1 = (-B - sqrt(det)) / (2 * A);
			double t2 = (-B + sqrt(det)) / (2 * A);
			if (t1 >= 0.01 && t2 >= 0.01)
			{
				return min(t1, t2);
			}
			else if (t1 >= 0.01)
			{
				return t1;
			}
			else if (t2 >= 0.01)
			{
				return t2;
			}
		}
	}
	return t;
}

Dvector *ImageCalc::calcNormal(Intersection &inter)
{
	Object *object = &(inter.getObject());
	Point *hit = &(inter.getHit());
	Dvector *normal = 0;
	if (object->getD() <= 0) // the object is a plane
	{
		double x = object->getLocation().getFirst();
		double y = object->getLocation().getSecond();
		double z = object->getLocation().getThird();
		normal = new Dvector(-x, -y, -z);
		normal->normalize();
	}
	else // the object is a sphere
	{
		normal = new Dvector((object->getLocation()), *hit);
		normal->normalize();
	}
	return normal;
}

Color *ImageCalc::calcDiffuseColor(const Ray &in_ray, Intersection &inter, Light &light)
{
	Dvector *oppositeLight;
	Color *final_color = new Color(0, 0, 0);
	Spotlight *spotlight = dynamic_cast<Spotlight *>(&light);
	if (spotlight)
	{
		Dvector *spotlightToHit = new Dvector(spotlight->getPosition(), inter.getHit());
		double light_angle = acos(dotProduct(*spotlightToHit, light.getDirectionVector()));
		delete spotlightToHit;
		if (acos(spotlight->getcosAngle()) < light_angle)
		{
			return final_color;
		}
	}
	if (spotlight)
	{
		oppositeLight = new Dvector(spotlight->getPosition(), inter.getHit());
	}
	else
	{
		oppositeLight = new Dvector(light.getDirectionVector());
	}
	oppositeLight->scale(-1.0);
	Dvector *normal = calcNormal(inter);
	double cosTheta = dotProduct(*normal, *oppositeLight);
	final_color->add(inter.getObject().getColor());
	final_color->scale(cosTheta);
	final_color->mult(light.getIntensity());
	delete normal;
	delete oppositeLight;
	final_color->clip();
	return final_color;
}

double ImageCalc::dotProduct(Dvector &first, Dvector &second)
{
	first.normalize();
	second.normalize();
	double res = 0;
	res += first.getFirst() * second.getFirst();
	res += first.getSecond() * second.getSecond();
	res += first.getThird() * second.getThird();
	return max(min(res, 1.0), -1.0);
}

double ImageCalc::distance(const Point &first, const Point &second)
{ // does not delete the vectors
	double x = first.getFirst() - second.getFirst();
	double y = first.getSecond() - second.getSecond();
	double z = first.getThird() - second.getThird();

	return sqrt(x * x + y * y + z * z);
}

Color *ImageCalc::calcSpecularColor(const Ray &in_ray, Intersection &inter, Light &light)
{
	Color *final_color = new Color(0, 0, 0);
	Dvector *lightCopy;
	Spotlight *spotlight = dynamic_cast<Spotlight *>(&light);
	if (spotlight)
	{
		Dvector *spotlightToHit = new Dvector(spotlight->getPosition(), inter.getHit());
		double light_angle = acos(dotProduct(*spotlightToHit, light.getDirectionVector()));
		delete spotlightToHit;
		if (acos(spotlight->getcosAngle()) < light_angle)
		{
			return final_color;
		}
	}
	if (spotlight)
	{
		lightCopy = new Dvector(spotlight->getPosition(), inter.getHit());
	}
	else
	{
		lightCopy = new Dvector(light.getDirectionVector());
	}
	Dvector *normal = calcNormal(inter);
	Dvector *in_ray_vector = new Dvector(in_ray.getVector());
	in_ray_vector->scale(-1.0);
	Dvector *reflective = calcReflective(*normal, *lightCopy);
	double n = inter.getObject().getShininess();
	double dot = max(dotProduct(*in_ray_vector, *reflective), 0.0);
	// If the light is positioned such that the reflection vector is pointing in the opposite direction to the view (in_ray) vector,
	// the dot product between the in_ray and reflection vectors will be negative, and the specular intensity will be 0
	double specular_scalar = pow(dot, n);
	final_color->add(light.getIntensity());
	final_color->scale(specular_scalar);
	final_color->mult(inter.getObject().getKs());
	delete in_ray_vector;
	delete normal;
	delete reflective;
	delete lightCopy;
	final_color->clip();
	return final_color;
}

Dvector *ImageCalc::calcReflective(Dvector &normal, Dvector &light_vector) // Compute the reflection vector: R = L - 2 * (L · N) * N
{
	double dot = dotProduct(light_vector, normal);
	Dvector *reflection = new Dvector(light_vector); // Copy light vector
	Dvector *temp = new Dvector(normal);			 // Copy normal vector
	temp->scale(-2.0 * dot);						 // Scale normal by -2 * dot product
	reflection->add(*temp);							 // Add scaled normal to light vector
	delete temp;
	return reflection;
}

void ImageCalc ::checkerboardColor(Color &color, Point &hitPoint)
{
	// Checkerboard pattern
	double scaleParameter = 0.5;
	double checkerboard = 0;
	if (hitPoint.getFirst() < 0)
	{
		checkerboard += floor((0.5 - hitPoint.getFirst()) / scaleParameter);
	}
	else
	{
		checkerboard += floor(hitPoint.getFirst() / scaleParameter);
	}
	if (hitPoint.getSecond() < 0)
	{
		checkerboard += floor((0.5 - hitPoint.getSecond()) / scaleParameter);
	}
	else
	{
		checkerboard += floor(hitPoint.getSecond() / scaleParameter);
	}
	checkerboard = (checkerboard * 0.5) - int(checkerboard * 0.5);
	checkerboard *= 2;
	if (checkerboard > 0.5)
	{
		color.scale(0.5);
	}
}
