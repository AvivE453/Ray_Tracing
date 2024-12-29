#include <iostream>
#include <fstream>
#include "Scene.h"
#include "Parse.h"
#include "Eye.h"
#include "Light.h"
#include "Object.h"

using namespace std;
int spotlightCounter = 0;

void Parse::ParseFile(const string &configorationFile, Scene &scene)
{
    ifstream myFile(configorationFile);
    string myline;

    while (getline(myFile, myline))
    {
        if (myline.rfind("e", 0) == 0)
        {
            vector<string> words = tokenize(myline, " ");
            Eye *eye = new Eye(stod(words[1]), stod(words[2]), stod(words[3])); // stod =string to double
            scene.addEye(eye);
        }
        else if (myline.rfind("a", 0) == 0)
        {
            vector<string> words = tokenize(myline, " ");
            scene.setAmbient(stod(words[1]), stod(words[2]), stod(words[3]));
        }

        else if (myline.rfind("o", 0) == 0)
        {
            vector<string> words = tokenize(myline, " ");
            if (stod(words[4]) > 0)
            {
                Sphere *sphere = new Sphere(stod(words[1]), stod(words[2]), stod(words[3]), stod(words[4]), 0);
                scene.addObject(sphere);
            }
            else
            {
                Plane *plane = new Plane(stod(words[1]), stod(words[2]), stod(words[3]), stod(words[4]), 0);
                scene.addObject(plane);
            }
        }
        else if (myline.rfind("r", 0) == 0)
        {
            vector<string> words = tokenize(myline, " ");
            if (stod(words[4]) > 0)
            {
                Sphere *sphere = new Sphere(stod(words[1]), stod(words[2]), stod(words[3]), stod(words[4]), 1);
                scene.addObject(sphere);
            }
            else
            {
                Plane *plane = new Plane(stod(words[1]), stod(words[2]), stod(words[3]), stod(words[4]), 1);
                scene.addObject(plane);
            }
        }
        else if (myline.rfind("t", 0) == 0)
        {
            vector<string> words = tokenize(myline, " ");
            if (stod(words[4]) > 0)
            {
                Sphere *sphere = new Sphere(stod(words[1]), stod(words[2]), stod(words[3]), stod(words[4]), 2);
                scene.addObject(sphere);
            }
            else
            {
                Plane *plane = new Plane(stod(words[1]), stod(words[2]), stod(words[3]), stod(words[4]), 2);
                scene.addObject(plane);
            }
        }
        else if (myline.rfind("c", 0) == 0)
        {
            vector<string> words = tokenize(myline, " ");
            scene.getNextObject().setColor(stod(words[1]), stod(words[2]), stod(words[3]), stod(words[4]));
        }
        else if (myline.rfind("d", 0) == 0)
        {

            vector<string> words = tokenize(myline, " ");
            if (stod(words[4]) == 0.0)
            {
                DirectionalLight *light = new DirectionalLight(stod(words[1]), stod(words[2]), stod(words[3]));
                scene.addLight(light);
            }
            else
            {
                spotlightCounter++;
                Spotlight *light = new Spotlight(stod(words[1]), stod(words[2]), stod(words[3]));
                scene.addSpotlight(light);
            }
        }
        else if (myline.rfind("p", 0) == 0)
        {
            vector<string> words = tokenize(myline, " ");
            scene.getNextSpotlight().setPositionAndCosAngle(stod(words[1]), stod(words[2]), stod(words[3]), stod(words[4]));
        }
        else if (myline.rfind("i", 0) == 0)
        {
            if (spotlightCounter > 0)
            {
                spotlightCounter--;
                vector<string> words = tokenize(myline, " ");
                scene.getNextSpotlight().setIntensity(stod(words[1]), stod(words[2]), stod(words[3]));
            }
            else
            {
                vector<string> words = tokenize(myline, " ");
                scene.getNextDirectionalLight().setIntensity(stod(words[1]), stod(words[2]), stod(words[3]));
            }
        }
        myFile >> ws;
        if (!myFile)
            break;
    }
    myFile.close();
}
vector<string> Parse::tokenize(const string &s, const string &del)
{
    vector<string> words;
    int start, end = -1 * del.size();
    do
    {
        start = end + del.size();
        end = s.find(del, start);
        if (s[start] == ' ' || s[start] == '#')
            break;
        words.push_back(s.substr(start, end - start));
    } while (end != -1);
    return words;
}
