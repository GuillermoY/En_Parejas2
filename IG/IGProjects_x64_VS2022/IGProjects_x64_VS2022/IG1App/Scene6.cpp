#include "Scene6.h"
#include "IndexedBox.h"

void Scene6::init()
{
    setGL();
    gObjects.push_back(new RGBAxes(400.0));
    gObjects.push_back(new IndexedBox(150.0));
}