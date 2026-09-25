#ifndef __IG2PROJECT_H__
#define __IG2PROJECT_H__

#include <OgreApplicationContext.h>
#include <OgreSceneManager.h>
#include <OgreRTShaderSystem.h>       
#include <OgreSceneNode.h>
#include <OgreTrays.h>
#include <OgreCameraMan.h>
#include <OgreEntity.h>
#include <OgreInput.h>
#include <OgreMeshManager.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include "Ogre.h"
#include "Simbad.h"
#include "Labyrinth.h"
#include <OgreWindowEventUtilities.h>
#include <SDL_keycode.h>


class IG2Project : public OgreBites::ApplicationContext, OgreBites::InputListener {

public:
    explicit IG2Project() : OgreBites::ApplicationContext("IG2Project") {};
    virtual ~IG2Project() {};

    //Directions
    typedef enum { UP, DOWN, LEFT, RIGHT } tDir; // Dentro de character 
    static const int SPEED; // Dentro de character

protected:
    virtual bool keyPressed(const OgreBites::KeyboardEvent& evt);
    virtual void frameRendered(const Ogre::FrameEvent& evt);
    virtual void setup();
    virtual void shutdown();
    virtual void setupScene();
    bool isDirectionModified(); // Dentro de character
    Vector3 getNexDirVector(); // Dentro de character
    Quaternion getQuaternionForNewDirection(); // Dentro de character

    Ogre::SceneNode* mSinbadNode = nullptr;
    tDir sinbadDirectorion = UP;

    Ogre::SceneManager* mSM = nullptr;
    OgreBites::TrayManager* mTrayMgr = nullptr;

    Ogre::Light* light = nullptr;
    Ogre::SceneNode* mLightParent = nullptr;
    Ogre::SceneNode* mLightNode = nullptr;

    Ogre::SceneNode* mCamNode = nullptr;
    OgreBites::CameraMan* mCamMgr = nullptr;

    Labyrinth* labyrinth;
    Simbad* heroe;
};

#endif
