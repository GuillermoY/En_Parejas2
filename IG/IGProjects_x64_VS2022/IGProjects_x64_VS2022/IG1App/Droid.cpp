#include "Droid.h"
#include "Cone.h"
#include "SphereWithTexture.h"
#include "Disk.h"

#include <glm/gtc/constants.hpp>

Droid::~Droid()
{
    ballTexture = nullptr;
    delete ballTexture;
}

Droid::Droid(GLdouble radius)
{
    ballTexture= new Texture();
    ballTexture->load("../assets/images/container.jpg");
    addEntity(new SphereWithTexture(radius, 150.0, 150.0, ballTexture));

    Cone* head = new Cone(radius/2, radius, radius / 2,radius);
    head->setModelMat(translate(head->modelMat(), glm::vec3(0.0f, radius, 0.0f)));
    addEntity(head);
    Disk* topHead = new Disk(radius/2,0.0,radius/2);
    topHead->setModelMat(translate(topHead->modelMat(), glm::vec3(0.0f, radius+radius/2, 0.0f)));
    topHead->setModelMat(rotate(topHead->modelMat(), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    addEntity(topHead);
    Disk* bottomHead = new Disk(radius, 0.0, radius);
    bottomHead->setModelMat(translate(bottomHead->modelMat(), glm::vec3(0.0f, radius, 0.0f)));
    bottomHead->setModelMat(rotate(bottomHead->modelMat(), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    addEntity(bottomHead);

    Cone* cannonL = new Cone(radius/2, radius / 10, radius / 10, radius);
    cannonL->setModelMat(translate(cannonL->modelMat(), glm::vec3(-radius/7, radius+radius/4, radius/2)));
    cannonL->setModelMat(rotate(cannonL->modelMat(), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    cannonL->setColor({ 0,0.8,0,1 });
    addEntity(cannonL);

    Disk* cannonLTap = new Disk(radius/10, 0.0, radius);
    cannonLTap->setModelMat(translate(cannonLTap->modelMat(), glm::vec3(-radius / 7, radius + radius / 4, radius)));
    cannonLTap->setModelMat(rotate(cannonLTap->modelMat(), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    cannonLTap->setColor({ 0,0.8,0,1 });
    addEntity(cannonLTap);

    Disk* cannonRTap = new Disk(radius / 10, 0.0, radius);
    cannonRTap->setModelMat(translate(cannonRTap->modelMat(), glm::vec3(radius / 7, radius + radius / 4, radius)));
    cannonRTap->setModelMat(rotate(cannonRTap->modelMat(), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    cannonRTap->setColor({ 0,0.8,0,1 });
    addEntity(cannonRTap);

    Cone* cannonR = new Cone(radius / 2, radius / 10, radius / 10, radius);
    cannonR->setModelMat(translate(cannonR->modelMat(), glm::vec3(radius / 7, radius + radius / 4, radius / 2)));
    cannonR->setModelMat(rotate(cannonR->modelMat(), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    cannonR->setColor({ 0,0.8,0,1 });
    addEntity(cannonR);
}