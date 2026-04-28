#include "Droid.h"

#include <glm/gtc/constants.hpp>

Droid::~Droid()
{
    delete ballTexture;
    ballTexture = nullptr;
}

Droid::Droid(GLdouble radius)
{
    ballTexture= new Texture();
    ballTexture->load("../assets/images/container.jpg");
    ballBody = new SphereWithTexture(radius, 150.0, 150.0, ballTexture);
    addEntity(ballBody);

    GLdouble halfRad = radius / 2;
    GLdouble quartRad = radius / 4;
    GLdouble seventRad = radius / 7;
    GLdouble tenRad = radius / 10;

    Cone* head = new Cone(halfRad, radius, halfRad,radius);
    head->setModelMat(translate(head->modelMat(), glm::vec3(0.0f, radius, 0.0f)));
    addEntity(head);
    Disk* topHead = new Disk(halfRad,0.0,halfRad);
    topHead->setModelMat(translate(topHead->modelMat(), glm::vec3(0.0f, radius+halfRad, 0.0f)));
    topHead->setModelMat(rotate(topHead->modelMat(), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    addEntity(topHead);
    Disk* bottomHead = new Disk(radius, 0.0, radius);
    bottomHead->setModelMat(translate(bottomHead->modelMat(), glm::vec3(0.0f, radius, 0.0f)));
    bottomHead->setModelMat(rotate(bottomHead->modelMat(), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    addEntity(bottomHead);

    Cone* cannonL = new Cone(halfRad, tenRad, tenRad, radius);
    cannonL->setModelMat(translate(cannonL->modelMat(), glm::vec3(-seventRad, radius+quartRad, halfRad)));
    cannonL->setModelMat(rotate(cannonL->modelMat(), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    cannonL->setMaterial(Material(glm::vec3{ 0,0.8,0 }));
    addEntity(cannonL);

    Disk* cannonLTap = new Disk(tenRad, 0.0, radius);
    cannonLTap->setModelMat(translate(cannonLTap->modelMat(), glm::vec3(-seventRad, radius + quartRad, radius)));
    cannonLTap->setModelMat(rotate(cannonLTap->modelMat(), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    cannonLTap->setMaterial(Material(glm::vec4{ 0,0.8,0,1 }));
    addEntity(cannonLTap);

    Disk* cannonRTap = new Disk(tenRad, 0.0, radius);
    cannonRTap->setModelMat(translate(cannonRTap->modelMat(), glm::vec3(seventRad, radius + quartRad, radius)));
    cannonRTap->setModelMat(rotate(cannonRTap->modelMat(), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    cannonRTap->setMaterial(Material(glm::vec4{ 0,0.8,0,1 }));
    addEntity(cannonRTap);

    Cone* cannonR = new Cone(halfRad, tenRad, tenRad, radius);
    cannonR->setModelMat(translate(cannonR->modelMat(), glm::vec3(seventRad, radius + quartRad, halfRad)));
    cannonR->setModelMat(rotate(cannonR->modelMat(), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    cannonR->setMaterial(Material(glm::vec4{ 0,0.8,0,1 }));
    addEntity(cannonR);
}

void Droid::rotateBall(float degrees)
{
    ballBody->setModelMat(rotate(ballBody->modelMat(), glm::radians(degrees), glm::vec3(1.0f, 0.0f, 0.0f)));
}