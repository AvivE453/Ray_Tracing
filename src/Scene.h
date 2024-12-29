#pragma once
#include <string>
#include <vector>
#include "Light.h"
#include "Object.h"
#include "Eye.h"
#include "Parse.h"

using namespace std;

class Scene
{
private:
    Parse parse;
    vector<Light *> directionalLights; // Circular vector of lights
    vector<Object *> objects;          // Circular vector of objects
    vector<Spotlight *> spotlights;    // Circular vector of spotlights
    int lightIndex;
    int objectIndex;
    int spotlightIndex;
    Color *ambient; // Global ambient intensity
    Eye *eye;

public:
    Scene(const string &configFilePath);
    Scene(const Scene &other);            // copy constructor
    Scene &operator=(const Scene &other); // copy assignment operator
    ~Scene();
    // Methods to get the next light, object, and spotlight
    Light &getNextDirectionalLight();
    Object &getNextObject();
    Spotlight &getNextSpotlight();
    Color &getAmbient();
    Eye &getEye() const;
    //  Methods to add objects and lights
    void addLight(Light *light);
    void addObject(Object *object);
    void addSpotlight(Spotlight *spotlight);
    void setAmbient(double _R, double _G, double _B);
    void addEye(Eye *_eye);
    size_t getObjectsSize() const;
    size_t getLightsSize() const;
    size_t getSpotlightsSize() const;
    Scene *clone() const;
};
