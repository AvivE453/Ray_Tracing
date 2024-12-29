#include <iostream>
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include "Parse.h"
#include "Scene.h"
#include "Image.h"
#include "ImageCalc.h"

using namespace std;

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "usage: scene <config_path>" << std::endl;
        return 0;
    }
    int result;
    string configurationFile = argv[1];
    int width = 800;
    int height = 800;
    int req_comps = 4;
    Scene *scene = new Scene(configurationFile);
    Image *image = new Image(width, height, req_comps);
    ImageCalc::RayCast(*scene, *image);
    result = stbi_write_png("res/textures/Scene.png", width, height, req_comps, image->getBuffer(), width * req_comps);
    std::cout << result << std::endl;
    delete scene;
    delete image;
    return 0;
}
