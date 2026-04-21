#include "SnowMan.h"
#include "Cone.h"
#include "Sphere.h"
#include "Disk.h"

#include <glm/gtc/constants.hpp>
SnowMan::SnowMan(GLdouble radius)
{
    Sphere* body = new Sphere(radius, radius, radius);
    addEntity(body);
    body->setColor({ 0.95,0.95,0.95,1 });

    Sphere* head = new Sphere(radius/1.5, radius, radius);
    addEntity(head);
    head->setModelMat(translate(head->modelMat(), glm::vec3(0.0f, radius*1.6, 0.0f)));
    head->setColor({ 0.95,0.95,0.95,1 });

    // hat
    CompoundEntity* hat = new CompoundEntity();

    Disk* bottomHat = new Disk(radius, 0.0, radius);
    bottomHat->setModelMat(translate(bottomHat->modelMat(), glm::vec3(0.0f, radius*1.4+radius/1.6, 0.0f)));
    bottomHat->setModelMat(rotate(bottomHat->modelMat(), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    hat->addEntity(bottomHat);
    bottomHat->setColor({ 0,0.95,1,1 });

    Cone* bodyHat = new Cone(radius / 2, radius/1.5, radius/1.5, radius);
    bodyHat->setModelMat(translate(bodyHat->modelMat(), glm::vec3(0.0f, radius*1.6+radius/1.5+radius/4, 0.0f)));
    bodyHat->setModelMat(rotate(bodyHat->modelMat(), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    hat->addEntity(bodyHat);
    bodyHat->setColor({ 0,0,1,1 });

    Disk* topHat = new Disk(radius/1.5, 0.0, radius);
    topHat->setModelMat(translate(topHat->modelMat(), glm::vec3(0.0f, radius * 1.6 + radius / 1.5 + radius / 4, 0.0f)));
    topHat->setModelMat(rotate(topHat->modelMat(), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    hat->addEntity(topHat);
    topHat->setColor({ 1,0,0,1 });
    addEntity(hat);

    // Cara
    Cone* nose = new Cone(radius / 4, radius / 15,0 , radius);
    nose->setModelMat(translate(nose->modelMat(), glm::vec3(0.0f, radius + radius/2, radius/2+radius/8)));
    nose->setModelMat(rotate(nose->modelMat(), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    addEntity(nose);
    nose->setColor({ 1,0.65,0,1 });

    Cone* eyeL = new Cone(radius / 8, radius / 8, 0, radius);
    eyeL->setModelMat(translate(eyeL->modelMat(), glm::vec3(-radius / 5, radius + radius / 1.3, radius / 2 + radius / 12)));
    eyeL->setModelMat(rotate(eyeL->modelMat(), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    eyeL->setModelMat(rotate(eyeL->modelMat(), glm::radians(15.0f), glm::vec3(-1.0f, 0.0f, 1.0f)));
    addEntity(eyeL);
    eyeL->setColor({ 0,0.5,0.5,1 });

    Cone* eyeR = new Cone(radius / 8, radius / 8, 0, radius);
    eyeR->setModelMat(translate(eyeR->modelMat(), glm::vec3(radius / 5, radius + radius / 1.3, radius / 2 + radius / 12)));
    eyeR->setModelMat(rotate(eyeR->modelMat(), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    eyeR->setModelMat(rotate(eyeR->modelMat(), glm::radians(15.0f), glm::vec3(-1.0f, 0.0f, -1.0f)));
    addEntity(eyeR);
    eyeR->setColor({ 0,0.5,0.5,1 });


}