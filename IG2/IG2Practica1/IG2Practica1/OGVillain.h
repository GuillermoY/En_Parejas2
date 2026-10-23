#pragma once
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreInput.h>
#include <OgreMath.h>
#include <OgreFrameListener.h>
#include <OgreMeshManager.h>
#include <SDL_keycode.h>
#include <iostream>
#include "Villain.h"

class OGVillain : public Villain {
public:
	OGVillain(Vector3 initPos, SceneNode* node, SceneManager* sceneMng, String headMesh)
		: Villain(initPos, node, sceneMng, headMesh, true){
        createModel();
	};

    void update(Ogre::Real time)
    {
        if (rotationTimer >= rotationDuration)
        {
            rotateForward = !rotateForward;
            rotationTimer = 0;
        }
        else
            rotationTimer++;

        animate(time);
    }

private:
    // Brazos
    Ogre::SceneNode* leftArmNode = nullptr;
    Ogre::SceneNode* rightArmNode = nullptr;

    // Piernas
    Ogre::SceneNode* leftLegNode = nullptr;
    Ogre::SceneNode* rightLegNode = nullptr;

    // Codos
    Ogre::SceneNode* leftUpperArmNode = nullptr;
    Ogre::SceneNode* rightUpperArmNode = nullptr;

    // Manos
    Ogre::SceneNode* leftForearmNode = nullptr;
    Ogre::SceneNode* rightForearmNode = nullptr;

    float rotationTimer = 0.0f;
    float rotationDuration = 50.0f;

    bool rotateForward = true;


    void createPart(Vector3 pos, Ogre::SceneNode*& part, Ogre::SceneNode* parent = nullptr,
        Vector3 scale = {0.01f,0.01f,0.01f}, string mat = "example/RustySteelM", string mesh = "cube.mesh")
    {
        Ogre::Entity* partEntity = sM->createEntity(mesh);
        if (mat != "")
            partEntity->setMaterialName(mat);

        if (parent == nullptr)
            part = this->createChildSceneNode();
        else
            part = parent->createChildSceneNode();


        part->attachObject(partEntity);
        part->setScale(scale);
        part->setPosition(pos);
    }

    void createModel()
    {
        //Brazo Izquiedo
        leftArmNode = createChildSceneNode();
        leftArmNode->setPosition(-2.0f, 2.0f, 0.0f);

        Ogre::SceneNode* leftShoulderNode = nullptr;
        createPart({ 0.0f, 0.0f, 0.0f }, leftShoulderNode, leftArmNode);
        createPart({ 0.0f, -1.0f, 0.0f }, leftUpperArmNode, leftArmNode, { 0.004f, 0.001f, 0.004f }, "example/BumpyMetalM");
        createPart({ 0.0f, -2.0f, 0.0f }, leftForearmNode, leftArmNode, { 0.25, 0.25, 0.25}, "", "fish.mesh");

        //Brazo Derecho
        rightArmNode = createChildSceneNode();
        rightArmNode->setPosition(2.0f, 2.0f, 0.0f);
        
        Ogre::SceneNode* rightShoulderNode = nullptr;
        createPart({ 0.0f, 0.0f, 0.0f }, rightShoulderNode, rightArmNode);
        createPart({ 0.0f, -1.0f, 0.0f }, rightUpperArmNode, rightArmNode, { 0.004f, 0.001f, 0.004f }, "example/BumpyMetalM");
        createPart({ 0.0f, -2.0f, 0.0f }, rightForearmNode, rightArmNode, { 0.007f, 0.004f, 0.007f }, "example/MtlPlat2M");
        
        //Torso
        Ogre::SceneNode* torso = nullptr;
        createPart({ 0.0f, 0.0f, 0.0f }, torso, nullptr, {0.4,0.4,0.4}, "", "Barrel.mesh");

        //Pierna Izquierda
        leftLegNode = createChildSceneNode();
        leftLegNode->setPosition(-2.0f, -2.0f, 0.0f);

        Ogre::SceneNode* leftLowerLegNode = nullptr;
        createPart({ 0.0f, 0.0f, 0.0f }, leftLowerLegNode, leftLegNode, { 0.01f,0.01f,0.01f }, "example/terr_rock6M");
        Ogre::SceneNode* leftUpperLegNode = nullptr;
        createPart({ 0.0f, -1.0f, 0.0f }, leftUpperLegNode, leftLegNode, { 0.007f, 0.004f, 0.007f });

        //Pierna Derecha
        rightLegNode = createChildSceneNode();
        rightLegNode->setPosition(2.0f, -2.0f, 0.0f);

        Ogre::SceneNode* rightLowerLegNode = nullptr;
        createPart({ 0.0f, 0.0f, 0.0f }, rightLowerLegNode, rightLegNode, { 0.01f,0.01f,0.01f },"example/terr_rock6M");
        Ogre::SceneNode* rightUpperLegNode = nullptr;
        createPart({ 0.0f, -1.f, 0.0f }, rightUpperLegNode, rightLegNode, { 0.007f, 0.004f, 0.007f });
    }

    void animate(Ogre::Real time)
    {
        if (leftArmNode && rightArmNode
            && leftLegNode && rightLegNode
            && leftUpperArmNode && rightUpperArmNode
            && leftForearmNode && rightForearmNode)
        {
            Ogre::Real direction = 1.0f;
            if (!rotateForward)
                direction = -1.0f;

            Ogre::Radian armsAngle = Ogre::Radian(1.0f * direction * time);

            leftArmNode->pitch(armsAngle);
            rightArmNode->pitch(-armsAngle);

            Ogre::Radian upperArmAngle = Ogre::Radian(5.0f * direction * time);

            leftUpperArmNode->pitch(-upperArmAngle);
            rightUpperArmNode->pitch(upperArmAngle);

            Ogre::Radian handAngle = Ogre::Radian(10.0f * direction * time);

            leftForearmNode->yaw(-upperArmAngle);
            rightForearmNode->yaw(upperArmAngle);
            
            Ogre::Radian legsAngle = Ogre::Radian(2.0f * direction * time);
            
            leftLegNode->pitch(-legsAngle);
            rightLegNode->pitch(legsAngle);

        }
    }
};