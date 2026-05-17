#include "Droid.h"

#include <glm/gtc/constants.hpp>

Droid::Droid(GLdouble radius)
{
    ballTexture= new Texture();
    ballTexture->load("../assets/images/container.jpg");
    addTexture(ballTexture);
    ballBody = new SphereWithTexture(radius, 150.0, 150.0, ballTexture);
    addEntity(ballBody);

    GLdouble halfRad = radius / 2;
    GLdouble quartRad = radius / 4;
    GLdouble seventRad = radius / 7;
    GLdouble tenRad = radius / 10;

    CompoundEntity* headEntity = new CompoundEntity();

    Cone* head = new Cone(halfRad, radius, halfRad,radius);
    head->setModelMat(translate(head->modelMat(), glm::vec3(0.0f, radius, 0.0f)));
    headEntity->addEntity(head);
    Disk* topHead = new Disk(0.0, halfRad,halfRad);
    topHead->setModelMat(translate(topHead->modelMat(), glm::vec3(0.0f, radius+halfRad, 0.0f)));
    topHead->setModelMat(rotate(topHead->modelMat(), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    headEntity->addEntity(topHead);
    Disk* bottomHead = new Disk(0.0, radius, radius);
    bottomHead->setModelMat(translate(bottomHead->modelMat(), glm::vec3(0.0f, radius, 0.0f)));
    bottomHead->setModelMat(rotate(bottomHead->modelMat(), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    headEntity->addEntity(bottomHead);

    Cone* cannonL = new Cone(halfRad, tenRad, tenRad, radius);
    cannonL->setModelMat(translate(cannonL->modelMat(), glm::vec3(-seventRad, radius+quartRad, halfRad)));
    cannonL->setModelMat(rotate(cannonL->modelMat(), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    cannonL->setMaterial(Material(glm::vec3{ 0,0.8,0 }));
    headEntity->addEntity(cannonL);

    Disk* cannonLTap = new Disk( 0.0, tenRad, radius);
    cannonLTap->setModelMat(translate(cannonLTap->modelMat(), glm::vec3(-seventRad, radius + quartRad, radius)));
    cannonLTap->setModelMat(rotate(cannonLTap->modelMat(), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    cannonLTap->setMaterial(Material(glm::vec4{ 0,0.8,0,1 }));
    headEntity->addEntity(cannonLTap);

    Disk* cannonRTap = new Disk( 0.0, tenRad, radius);
    cannonRTap->setModelMat(translate(cannonRTap->modelMat(), glm::vec3(seventRad, radius + quartRad, radius)));
    cannonRTap->setModelMat(rotate(cannonRTap->modelMat(), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    cannonRTap->setMaterial(Material(glm::vec4{ 0,0.8,0,1 }));
    headEntity->addEntity(cannonRTap);

    Cone* cannonR = new Cone(halfRad, tenRad, tenRad, radius);
    cannonR->setModelMat(translate(cannonR->modelMat(), glm::vec3(seventRad, radius + quartRad, halfRad)));
    cannonR->setModelMat(rotate(cannonR->modelMat(), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    cannonR->setMaterial(Material(glm::vec4{ 0,0.8,0,1 }));
    headEntity->addEntity(cannonR);

    addEntity(headEntity);

    // AP 79
    mDroidLight = new SpotLight(glm::vec3 { 0, radius, 0 }, 1);
    mDroidLight->setDirection(glm::vec3(0.0f, -1.0f, 0.0f));
    mDroidLight->setAmb(glm::vec3(0.25f, 0.25f, 0.25f));
    mDroidLight->setDiff(glm::vec3(0.6f, 0.6f, 0.6f));
    mDroidLight->setSpec(glm::vec3(0.0f, 1.0f, 0.0f));
    mDroidLight->setCutoff(16.0f, 17.0f);
    mDroidLight->setEnabled(true);
    addLight(mDroidLight);
}

void Droid::rotateBall(float degrees)
{
    ballBody->setModelMat(rotate(ballBody->modelMat(), glm::radians(degrees), glm::vec3(1.0f, 0.0f, 0.0f)));
}