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
    enum currScene
    {
        INTRO,
        GAME
    };
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
    void showCurrentInfo();

    Ogre::SceneNode* mSinbadNode = nullptr;

    Ogre::SceneManager* mSM = nullptr;
    OgreBites::TrayManager* mTrayMgr = nullptr;

    Ogre::Light* light = nullptr;
    Ogre::SceneNode* mLightParent = nullptr;
    Ogre::SceneNode* mLightNode = nullptr;

    Ogre::SceneNode* mCamNode = nullptr;
    OgreBites::CameraMan* mCamMgr = nullptr;

    Labyrinth* labyrinth = nullptr;
    Simbad* heroe = nullptr;

    OgreBites::Label* mInfoLabel = nullptr;
    OgreBites::TextBox* mInfoBox = nullptr;

    //CreateIntro
    void createIntroScene();
    void createGameScene();
    void changeScene();
    currScene actScene = INTRO;
    Ogre::SceneNode* mSinbadIntroNode = nullptr;
    Ogre::SceneNode* mFloorIntroNode = nullptr;

    void sinbadIntroAnimation();
    void addFrame(NodeAnimationTrack* nAnimTrack, float giro, Vector3 pos, Vector3 escala, float tiempo);
    Ogre::NodeAnimationTrack* sinbadAnim = nullptr;
    Ogre::AnimationState* sinbadAnimState = nullptr;
    Ogre::Animation* sinbadParent = nullptr;

    bool hasSwords = false;
};

#endif
