#include "Scene6.h"
#include "IndexedBox.h"
//#include "Cone.h"
//#include "Sphere.h"
//#include "Disk.h"

void Scene6::init()
{
    setGL();
    gObjects.push_back(new RGBAxes(400.0));
    gObjects.push_back(new IndexedBox(150.0));
    //gObjects.push_back(new Cone(150.0, 150.0, 150.0,150.0,150.0));
    //gObjects.push_back(new Sphere(150.0, 150.0,150.0));
    //gObjects.push_back(new Disk(150.0,150.0,150.0,150.0));
}