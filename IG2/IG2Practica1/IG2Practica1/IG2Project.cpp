#include "IG2Project.h"

using namespace std;
using namespace Ogre;

const int IG2Project::SPEED = 100;

bool IG2Project::keyPressed(const OgreBites::KeyboardEvent& evt) {

    // ESC key finished the rendering...
    if (evt.keysym.sym == SDLK_ESCAPE) {
        getRoot()->queueEndRendering();
    }

    else if (evt.keysym.sym == SDLK_k) {
        cout << "Position of Sinbad: " << mSinbadNode->getPosition() << endl;
        cout << "Position of the camera: " << mCamNode->getPosition() << endl;
    }

    else if (evt.keysym.sym == SDLK_UP) { //rotateToNewDirection character
        cout << "Pressed UP" << endl;
        heroe->changeDirection(heroe->UP);
    }
    else if (evt.keysym.sym == SDLK_DOWN) {
        cout << "Pressed DOWN" << endl;
        heroe->changeDirection(heroe->DOWN);
    }
    else if (evt.keysym.sym == SDLK_LEFT) {
        cout << "Pressed LEFT" << endl;
        heroe->changeDirection(heroe->LEFT);
    }
    else if (evt.keysym.sym == SDLK_RIGHT) {
        cout << "Pressed RIGHT" << endl;
        heroe->changeDirection(heroe->RIGHT);
    }
    else if (evt.keysym.sym == SDLK_s && actScene == INTRO)
    {
        actScene = GAME;
        changeScene();
    }

    return true;
}


void IG2Project::shutdown() {

    mShaderGenerator->removeSceneManager(mSM);
    mSM->removeRenderQueueListener(mOverlaySystem);

    mRoot->destroySceneManager(mSM);

    delete mTrayMgr;  mTrayMgr = nullptr;
    delete mCamMgr; mCamMgr = nullptr;

    // do not forget to call the base 
    OgreBites::ApplicationContext::shutdown();
}

void IG2Project::setup(void) {

    // do not forget to call the base first
    OgreBites::ApplicationContext::setup();

    // Create the scene manager
    mSM = mRoot->createSceneManager();

    // Register our scene with the RTSS
    mShaderGenerator->addSceneManager(mSM);
        
    mSM->addRenderQueueListener(mOverlaySystem);
    //mTrayMgr = new OgreBites::TrayManager("TrayGUISystem", mWindow.render);
    mTrayMgr = new OgreBites::TrayManager("TrayGUISystem", getRenderWindow());
    mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    addInputListener(mTrayMgr);

    // Adds the listener for this object
    addInputListener(this);
    setupScene();
}

void IG2Project::frameRendered(const Ogre::FrameEvent& evt) {
    if (actScene == GAME && labyrinth != nullptr && heroe != nullptr) {
        labyrinth->moveCharacter(heroe, evt.timeSinceLastFrame);
        labyrinth->update(evt.timeSinceLastFrame);
        showCurrentInfo();
    }
    else if (actScene == INTRO && sinbadAnimState != nullptr)
    {
        sinbadAnimState->addTime(evt.timeSinceLastFrame);
    }
}

void IG2Project::showCurrentInfo()
{
    std::stringstream ss;
    ss << "Lives: " << heroe->getRemainingLives() << "\n";
    ss << "Points: " << heroe->getPoints();
    mInfoBox->setText(ss.str());
}

void IG2Project::setupScene(void) {

    //------------------------------------------------------------------------
    // Creating the camera

    Camera* cam = mSM->createCamera("Cam");
    cam->setNearClipDistance(1);
    cam->setFarClipDistance(10000);
    cam->setAutoAspectRatio(true);
    //cam->setPolygonMode(Ogre::PM_WIREFRAME);

    mCamNode = mSM->getRootSceneNode()->createChildSceneNode("nCam");
    mCamNode->attachObject(cam);
    
    Viewport* vp = getRenderWindow()->addViewport(cam);

    mCamMgr = new OgreBites::CameraMan(mCamNode);
    addInputListener(mCamMgr);
    mCamMgr->setStyle(OgreBites::CS_ORBIT);

    createIntroScene();
#pragma region TestCommands

    //mSM->setAmbientLight(ColourValue(0.5, 0.5, 0.5));

    //Light* luz = mSM->createLight("Luz");
    //luz->setType(Ogre::Light::LT_DIRECTIONAL);
    //luz->setDiffuseColour(0.75, 0.75, 0.75);

    //mLightNode = mSM->getRootSceneNode()->createChildSceneNode("nLuz");
    //mLightNode->attachObject(luz);
    //mLightNode->setDirection(Ogre::Vector3(-1, -1, -1));
    
    //mSinbadNode = mSM->getRootSceneNode()->createChildSceneNode("nSinbad");
    //sinbad = new IG2Object(Vector3(0, 0, 0),
    //    mSinbadNode,
    //    mSM,
    //    "Sinbad.mesh");
    
    ////mSinbadNode->showBoundingBox(true);
    //sinbad->setScale(Vector3(15, 15, 15));
    //sinbad->move(Vector3(0, sinbad->calculateBoxSize().y / 2 + 1, 0));
    //Ogre::Entity* ent = mSM->createEntity("Sinbad.mesh");
    //mSinbadNode = mSM->getRootSceneNode()->createChildSceneNode("nSinbad");
    //mSinbadNode->attachObject(ent);

    // Show bounding box
    //mSinbadNode->showBoundingBox(true);

    // Set position of Sinbad
    //mSinbadNode->setPosition(x, y, z);

    // Set scale of Sinbad
    //mSinbadNode->setScale(20, 20, 20);

    //mSinbadNode->yaw(Ogre::Degree(-45));
    //mSinbadNode->setVisible(false);    


    //------------------------------------------------------------------------
    // Creating the floor

    //MeshManager::getSingleton().createPlane("floor", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
    //    Plane(Vector3::UNIT_Y, 0),
    //    1500, 1500, 50, 50, true, 1, 5, 5,
    //    Vector3::UNIT_Z);

    //Entity* entFloor = mSM->createEntity("exampleFloor", "floor");
    //entFloor->setMaterialName("example/stonesFloor");
    //SceneNode* floorNode = mSM->getRootSceneNode()->createChildSceneNode();
    //floorNode->attachObject(entFloor);


    //------------------------------------------------------------------------
   // Creating the dragon

    //Ogre::Entity* entDragon = mSM->createEntity("cube.mesh");
    //Ogre::SceneNode* mDragonNode = mSM->getRootSceneNode()->createChildSceneNode("nDragon");
    //mDragonNode->attachObject(entDragon);

    //// Show bounding box
    //mDragonNode->showBoundingBox(true);

    // Set position of the dragon
    //mDragonNode->setPosition(x, y, z);

    // Set scale of the dragon
    //mDragonNode->setScale(20, 20, 20);

    //mDragonNode->yaw(Ogre::Degree(-45));
    //mDragonNode->setVisible(false);
#pragma endregion
}

void IG2Project::changeScene()
{
    if (mSinbadIntroNode)
    {
        mSinbadIntroNode->flipVisibility();
    }
    if (mLightNode)
    {
        mLightNode->flipVisibility();
    }
    if (mFloorIntroNode)
    {
        mFloorIntroNode->flipVisibility();
    }
    
    createGameScene();
}

void IG2Project::createIntroScene()
{
    mCamNode->setPosition(0, 500, 500);
    mCamNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_WORLD);

    //------------------------------------------------------------------------
    //Creating Light
    mSM->setAmbientLight(ColourValue(0.5, 0.5, 0.5));

    Light* luz = mSM->createLight("Luz");
    luz->setType(Ogre::Light::LT_DIRECTIONAL);
    luz->setDiffuseColour(0.75, 0.75, 0.75);

    mLightNode = mSM->getRootSceneNode()->createChildSceneNode("nLuz");
    mLightNode->attachObject(luz);
    mLightNode->setDirection(Ogre::Vector3(-1, -1, -1));

    //------------------------------------------------------------------------
    //Creating Sinbad

    Ogre::Entity* ent = mSM->createEntity("Sinbad.mesh");
    mSinbadIntroNode = mSM->getRootSceneNode()->createChildSceneNode("nSinbadIntro");
    mSinbadIntroNode->attachObject(ent);
    sinbadAnimState = ent->getAnimationState("Dance");// mSM->createAnimationState("sinbadIntro");

    //mSinbadIntroNode->showBoundingBox(true);
    
    //Set position of Sinbad
    mSinbadIntroNode->setPosition(0, 100, 0);
    
    //Set scale of Sinbad
    mSinbadIntroNode->setScale(20, 20, 20);

    sinbadIntroAnimation();
    
    //------------------------------------------------------------------------
    //Creating the floor
    
    MeshManager::getSingleton().createPlane("floor", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        Plane(Vector3::UNIT_Y, 0),
        1500, 1500, 50, 50, true, 1, 5, 5,
        Vector3::UNIT_Z);
    
    Entity* entFloor = mSM->createEntity("exampleFloor", "floor");
    entFloor->setMaterialName("example/stonesFloor");
    mFloorIntroNode = mSM->getRootSceneNode()->createChildSceneNode();
    mFloorIntroNode->attachObject(entFloor);
}

void IG2Project::sinbadIntroAnimation()
{
    sinbadParent = mSM->createAnimation("sinbadIntro", 21.0f);
    sinbadParent->setInterpolationMode(Ogre::Animation::IM_SPLINE);
    sinbadAnim = sinbadParent->createNodeTrack(0, mSinbadIntroNode);

    //mSinbadIntroNode->setInitialState();

    //Vector3 keyframePos(0, 0, 0);
    //int movementLength = 50;
    //Real duration = 16.0;
    //Real durStep = duration / 4.0;

    //addFrame(sinbadAnim, 0, keyframePos, {20,20,20}, 0);
    //keyframePos += Ogre::Vector3::UNIT_X * movementLength;
    //addFrame(sinbadAnim, 0, keyframePos, { 20,20,20 }, durStep);
    //keyframePos += Ogre::Vector3::NEGATIVE_UNIT_X * movementLength;
    //addFrame(sinbadAnim, 0, keyframePos, { 20,20,20 }, durStep*2);
    //keyframePos += Ogre::Vector3::NEGATIVE_UNIT_X * movementLength;
    //addFrame(sinbadAnim, 0, keyframePos, { 20,20,20 }, durStep*3);
    //keyframePos += Ogre::Vector3::UNIT_X * movementLength;
    //addFrame(sinbadAnim, 0, keyframePos, { 20,20,20 }, durStep*4);

    sinbadAnimState->setLoop(true);
    sinbadAnimState->setEnabled(true);
}

void IG2Project::addFrame(NodeAnimationTrack* nAnimTrack, float giro, Vector3 pos, Vector3 escala, Real tiempo)
{
    TransformKeyFrame* kf;
    // Keyframe 0 (Init state)
    kf = nAnimTrack->createNodeKeyFrame(tiempo * 0);
    kf->setTranslate(pos);
}

void IG2Project::createGameScene()
{
    // and tell it to render into the main window
    //Viewport* vp = getRenderWindow()->addViewport(cam);
    mCamNode->setPosition(1900, 5000, 1900);
    mCamNode->lookAt({ 1900, 0, 1900 }, Ogre::Node::TS_WORLD);
    mCamNode->roll(Degree(180.0f));
    //------------------------------------------------------------------------
    // Creating the light
    //mSM->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);

    //------------------------------------------------------------------------
    // Creating Sinbad
    mSinbadNode = mSM->getRootSceneNode()->createChildSceneNode();
    heroe = new Simbad({ 0, 0, 0 }, mSinbadNode, mSM, "Sinbad.mesh");
    heroe->setScale(Vector3(20, 20, 20));
    mSinbadNode->showBoundingBox(true);

    //------------------------------------------------------------------------
    // Creating labyrinth

    labyrinth = new Labyrinth();
    labyrinth->createLabyrinth("stage1.txt", mSM, heroe);

    //------------------------------------------------------------------------
    // HUD
    mInfoLabel = mTrayMgr->createLabel(OgreBites::TL_BOTTOMRIGHT, "InfoLabel", "Stage: 1", 200);
    mInfoBox = mTrayMgr->createTextBox(OgreBites::TL_BOTTOMRIGHT, "InfoBox", "Status", 200, 100);
    showCurrentInfo();
}