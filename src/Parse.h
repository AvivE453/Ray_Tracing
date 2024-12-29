#pragma once
#include <string>
#include <vector>
using std::string;
using std::vector;

class Scene;
class Parse
{
public:
    void ParseFile(const string &configorationFile, Scene &scene);
    vector<string> tokenize(const string &s, const string &del);
};