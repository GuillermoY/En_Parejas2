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
    else if (evt.keysym.sym == SDLK_UP) {
        cout << "Pressed UP" << endl;
        sinbadDirectorion = UP;
    }
    else if (evt.keysym.sym == SDLK_DOWN) {
        cout << "Pressed DOWN" << endl;
        sinbadDirectorion = DOWN;
    }
    else if (evt.keysym.sym == SDLK_LEFT) {
        cout << "Pressed LEFT" << endl;
        sinbadDirectorion = LEFT;
    }
    else if (evt.keysym.sym == SDLK_RIGHT) {
        cout << "Pressed RIGHT" << endl;
        sinbadDirectorion = RIGHT;
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

bool IG2Project::isDirectionModified() {
    return heroe->getGridOrientation() != getNexDirVector();
}


Vector3 IG2Project::getNexDirVector() {

    Vector3 newDirVector = Vector3::ZERO;

    if (sinbadDirectorion == RIGHT)
        newDirVector = Vector3::UNIT_X;
    else if (sinbadDirectorion == LEFT)
        newDirVector = Vector3::NEGATIVE_UNIT_X;
    else if (sinbadDirectorion == DOWN)
        newDirVector = Vector3::UNIT_Z;
    else if (sinbadDirectorion == UP)
        newDirVector = Vector3::NEGATIVE_UNIT_Z;

    return newDirVector;
}

Quaternion IG2Project::getQuaternionForNewDirection() {

    Vector3 newDirVector = getNexDirVector();
    Quaternion q = heroe->getOrientation().getRotationTo(newDirVector);
    return q;
}

void IG2Project::frameRendered(const Ogre::FrameEvent& evt) {

    if (heroe != nullptr) {
        if (!isDirectionModified())
            heroe->move(getNexDirVector() * SPEED * evt.timeSinceLastFrame);
        else
            heroe->rotate(getQuaternionForNewDirection());
    }
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

    mCamNode->setPosition(0, 0, 1000);
    mCamNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_WORLD);

    // and tell it to render into the main window
    Viewport* vp = getRenderWindow()->addViewport(cam);

    mCamMgr = new OgreBites::CameraMan(mCamNode);
    addInputListener(mCamMgr);
    mCamMgr->setStyle(OgreBites::CS_ORBIT);


    //------------------------------------------------------------------------
    // Creating the light

    //mSM->setAmbientLight(ColourValue(0.5, 0.5, 0.5));
    
    Light* luz = mSM->createLight("Luz");
    luz->setType(Ogre::Light::LT_DIRECTIONAL);
    luz->setDiffuseColour(0.75, 0.75, 0.75);

    mLightNode = mSM->getRootSceneNode()->createChildSceneNode("nLuz");
    mLightNode->attachObject(luz);
    mLightNode->setDirection(Ogre::Vector3(-1, -1, -1));


    //------------------------------------------------------------------------
    // Creating Sinbad
    mSinbadNode = mSM->getRootSceneNode()->createChildSceneNode();
    heroe = new Simbad({0, 0, 0}, mSinbadNode, mSM, "Sinbad.mesh");
    heroe->setScale(Vector3(20, 20, 20));
    mSinbadNode->showBoundingBox(true);
    //mSinbadNode = mSM->getRootSceneNode()->createChildSceneNode("nSinbad");
    //sinbad = new IG2Object(Vector3(0, 0, 0),
    //    mSinbadNode,
    //    mSM,
    //    "Sinbad.mesh");

    //------------------------------------------------------------------------
    // Creating labyrinth

    labyrinth = new Labyrinth();
    labyrinth->createLabyrinth("stage1.txt", mSM, heroe);

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
}