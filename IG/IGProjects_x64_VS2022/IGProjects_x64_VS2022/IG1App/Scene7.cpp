#include "Scene7.h"
#include "SphereWithTexture.h"
#include "Droid.h"

void Scene7::init()
{
    gObjects.push_back(new Droid(150.0f));

    //gObjects.push_back(new Cone(150.0, 150.0, 150.0,150.0,150.0));
    //gObjects.push_back(new SphereWithTexture(150.0, 10,150.0,contTex));
    //gObjects.push_back(new Disk(150.0,150.0,150.0,150.0));
}