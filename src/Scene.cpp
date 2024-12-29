#include "Scene.h"
#include <stdexcept>

Scene::Scene(const string &configFilePath) : parse(), directionalLights(), objects(), spotlights(),
                                             lightIndex(0),
                                             objectIndex(0),
                                             spotlightIndex(0),
                                             ambient(new Color()), eye(new Eye())
{
    parse.ParseFile(configFilePath, *this);
}

Scene::Scene(const Scene &other) : parse(other.parse), directionalLights(), objects(), spotlights(),
                                   lightIndex(other.lightIndex), objectIndex(other.objectIndex),
                                   spotlightIndex(other.spotlightIndex), ambient(other.ambient->clone()), eye(other.eye->clone())
{
    for (size_t i = 0; i < other.directionalLights.size(); i++)
    {
        directionalLights.push_back(other.directionalLights.at(i)->clone());
    }
    for (size_t i = 0; i < other.spotlights.size(); i++)
    {
        spotlights.push_back(other.spotlights.at(i)->clone());
    }
    for (size_t i = 0; i < other.objects.size(); i++)
    {
        objects.push_back(other.objects.at(i)->clone());
    }
}

Scene &Scene::operator=(const Scene &other)
{
    if (this != &other)
    {
        parse = other.parse;
        lightIndex = other.lightIndex;
        objectIndex = other.objectIndex;
        spotlightIndex = other.spotlightIndex;
        delete ambient;
        ambient = other.ambient->clone();
        delete eye;
        eye = other.eye->clone();

        for (size_t i = 0; i < directionalLights.size(); i++)
        {
            delete directionalLights.at(i);
        }
        directionalLights.clear();
        for (size_t i = 0; i < other.directionalLights.size(); i++)
        {
            directionalLights.push_back(other.directionalLights.at(i)->clone());
        }

        for (size_t i = 0; i < objects.size(); i++)
        {
            delete objects.at(i);
        }
        objects.clear();
        for (size_t i = 0; i < other.objects.size(); i++)
        {
            objects.push_back(other.objects.at(i)->clone());
        }

        for (size_t i = 0; i < spotlights.size(); i++)
        {
            delete spotlights.at(i);
        }
        spotlights.clear();
        for (size_t i = 0; i < other.spotlights.size(); i++)
        {
            spotlights.push_back(other.spotlights.at(i)->clone());
        }
    }
    return *this;
}

// Destructor deletes all items from the lists and clears them
Scene::~Scene()
{
    delete ambient;
    ambient = nullptr;
    delete eye;
    eye = nullptr;

    for (size_t i = 0; i < directionalLights.size(); i++)
    {
        delete directionalLights.at(i);
    }
    directionalLights.clear();

    for (size_t i = 0; i < objects.size(); i++)
    {
        delete objects.at(i);
    }
    objects.clear();

    for (size_t i = 0; i < spotlights.size(); i++)
    {
        delete spotlights.at(i);
    }
    spotlights.clear();
}

// Method to get the next light in the circular list
Light &Scene::getNextDirectionalLight()
{
    if (directionalLights.empty())
    {
        throw std::runtime_error("No lights available in the scene.");
    }
    if (lightIndex == (int)directionalLights.size())
    {
        lightIndex = 0;
    }
    Light *light = directionalLights.at(lightIndex);
    lightIndex++;
    return *light;
}

// Method to get the next object in the circular list
Object &Scene::getNextObject()
{
    if (objects.empty())
    {
        throw std::runtime_error("No lights available in the scene.");
    }
    if (objectIndex == (int)objects.size())
    {
        objectIndex = 0;
    }
    Object *object = objects.at(objectIndex);
    objectIndex++;
    return *object;
}

// Method to get the next spotlight in the circular list
Spotlight &Scene::getNextSpotlight()
{
    if (spotlights.empty())
    {
        throw std::runtime_error("No lights available in the scene.");
    }
    if (spotlightIndex == (int)spotlights.size())
    {
        spotlightIndex = 0;
    }
    Spotlight *spotlight = spotlights.at(spotlightIndex);
    spotlightIndex++;
    return *spotlight;
}

Color &Scene::getAmbient()
{
    return *ambient;
}

Eye &Scene::getEye() const
{
    return *eye;
}

// Method to add a light to the list
void Scene::addLight(Light *light)
{
    directionalLights.push_back(light);
}

// Method to add an object to the list
void Scene::addObject(Object *object)
{
    objects.push_back(object);
}

// Method to add a spotlight to the list
void Scene::addSpotlight(Spotlight *spotlight)
{
    spotlights.push_back(spotlight);
}

void Scene::setAmbient(double _R, double _G, double _B)
{
    Color *temp = ambient;
    ambient = new Color(_R, _G, _B);
    delete (temp);
}

void Scene::addEye(Eye *_eye)
{
    Eye *temp = eye;
    eye = _eye;
    delete (temp);
}

size_t Scene::getObjectsSize() const
{
    return objects.size();
}

size_t Scene::getLightsSize() const
{
    return directionalLights.size();
}

size_t Scene::getSpotlightsSize() const
{
    return spotlights.size();
}

Scene *Scene::clone() const
{
    return new Scene(*this);
}
