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
#include "InvisibleBlock.h"
#include "FakeBlock.h"
#include "BreakableBlock.h"
#include "Simbad.h"
#include "Villain.h"

using namespace Ogre;

class Labyrinth {
public:
    void addBlock(Block* block, int row, int col, Vector3 tam);
    void addVillain(Villain* villain, int row, int col);
    void createLabyrinth(std::string stageFileName, SceneManager* SceneManager, Simbad* heroe);
    void moveCharacter(Character* character, Real time);
    void moveVillain(Villain* character, Real time);
    void update(Real time);

protected:
    int numRows;
    int numCols;
    const char WALL_BLOCK = 'x';
    const char EMPTY_BLOCK = 'o';
    const char INVISIBLE_BLOCK = 'i';
    const char FAKE_BLOCK = 'f';
    const char BREAKABLE_BLOCK = 'b';
    const char HERO_CELL = 'h';
    const char VILLAIN_CELL = 'v';
    const char OG_VILLAIN_CELL = 'V';
    std::ifstream stageFile;
    char cell;
    Block* block;
    const float BLOCK_SIZE = 200.0f;
    SceneManager* mSM = nullptr;
    std::vector<std::vector<Block*>> blocks;
    std::vector<Villain*> villains;

    SceneNode* SN;

    Block* getBlock(Vector3 position);
    void stepForward(Character* character, Block* charBlock, Block* inFrontBlock, Real time);
    bool blockCenterReached(Vector3 difference, Vector3 direction, float tolerance);
};
