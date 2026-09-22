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
#include <fstream>
#include "Wall.h"
#include "Empty.h"

using namespace Ogre;

class Labyrinth {
public:
    void addBlock(Block* block);
    void createLabyrinth(std::string stageFileName, SceneManager* SceneManager);
protected:
    int numRows;
    int numCols;
    const char WALL_BLOCK = 'x';
    const char EMPTY_BLOCK = 'o';
    std::ifstream stageFile;
    char cell;
    Block* block;
    const float BLOCK_SIZE = 30.0f;
    SceneManager* mSM = nullptr;
    std::vector<Block*> blocks;

    SceneNode* SN;
};
