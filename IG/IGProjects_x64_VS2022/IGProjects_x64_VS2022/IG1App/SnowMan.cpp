#include "SnowMan.h"
#include "Cone.h"
#include "Sphere.h"
#include "Disk.h"

#include <glm/gtc/constants.hpp>
SnowMan::SnowMan(GLdouble radius)
{
    GLdouble headRad = radius / 1.5;
    GLdouble halfRad = radius / 2;
    GLdouble quartRad = radius / 4;
    GLdouble fifthRad = radius / 5;
    GLdouble seventRad = radius / 7;
    GLdouble octRad = radius / 8;
    GLdouble twelftRad = radius / 12;
    GLdouble fifteentRad = radius / 15;

    Sphere* body = new Sphere(radius, radius, radius);
    addEntity(body);
    body->setColor({ 0.95,0.95,0.95,1 });

    Sphere* head = new Sphere(headRad, radius, radius);
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

    Cone* bodyHat = new Cone(halfRad, headRad, headRad, radius);
    bodyHat->setModelMat(translate(bodyHat->modelMat(), glm::vec3(0.0f, radius*1.6+ headRad +quartRad, 0.0f)));
    bodyHat->setModelMat(rotate(bodyHat->modelMat(), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    hat->addEntity(bodyHat);
    bodyHat->setColor({ 0,0,1,1 });

    Disk* topHat = new Disk(headRad, 0.0, radius);
    topHat->setModelMat(translate(topHat->modelMat(), glm::vec3(0.0f, radius * 1.6 + headRad + quartRad, 0.0f)));
    topHat->setModelMat(rotate(topHat->modelMat(), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    hat->addEntity(topHat);
    topHat->setColor({ 1,0,0,1 });
    addEntity(hat);

    // Cara
    Cone* nose = new Cone(quartRad, fifteentRad,0 , radius);
    nose->setModelMat(translate(nose->modelMat(), glm::vec3(0.0f, radius + halfRad, halfRad +radius/8)));
    nose->setModelMat(rotate(nose->modelMat(), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    addEntity(nose);
    nose->setColor({ 1,0.65,0,1 });

    Cone* eyeL = new Cone(octRad, octRad, 0, radius);
    eyeL->setModelMat(translate(eyeL->modelMat(), glm::vec3(-fifthRad, radius + radius / 1.3, halfRad + twelftRad)));
    eyeL->setModelMat(rotate(eyeL->modelMat(), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    eyeL->setModelMat(rotate(eyeL->modelMat(), glm::radians(15.0f), glm::vec3(-1.0f, 0.0f, 1.0f)));
    addEntity(eyeL);
    eyeL->setColor({ 0,0.5,0.5,1 });

    Cone* eyeR = new Cone(octRad, octRad, 0, radius);
    eyeR->setModelMat(translate(eyeR->modelMat(), glm::vec3(fifthRad, radius + radius / 1.3, halfRad + twelftRad)));
    eyeR->setModelMat(rotate(eyeR->modelMat(), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    eyeR->setModelMat(rotate(eyeR->modelMat(), glm::radians(15.0f), glm::vec3(-1.0f, 0.0f, -1.0f)));
    addEntity(eyeR);
    eyeR->setColor({ 0,0.5,0.5,1 });


}