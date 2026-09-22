#include "Labyrinth.h"

void Labyrinth::addBlock(Block* block)
{
    blocks.push_back(block);
}

void Labyrinth::createLabyrinth(std::string stageFileName, SceneManager* SM)
{
    mSM = SM;
    SN = mSM->getRootSceneNode()->createChildSceneNode();

    stageFile.open(stageFileName);
    // Read the number of files and columns
    stageFile >> numRows;
    stageFile >> numCols;

    bool ok = true;
    int iRow = 0;
    int iCol = 0;

    SceneNode* nodoTmp = SN->createChildSceneNode();
    Wall* referencia = new Wall(Vector3::ZERO, nodoTmp, mSM, "cube.mesh");
    Vector3 tam = referencia->calculateBoxSize();
    float propX = BLOCK_SIZE / tam.x;
    float propY = BLOCK_SIZE / tam.y;
    float propZ = BLOCK_SIZE / tam.z;
    delete referencia;

    while (iRow < numRows && ok) {
        iCol = 0;
        while (iCol < numCols && ok) {
            stageFile >> cell;
            // Inserts an empty block!
            if (cell == EMPTY_BLOCK) {
                SceneNode* nodoTmp = SN->createChildSceneNode();
                block = new Empty({ iCol * BLOCK_SIZE, 0, iRow * BLOCK_SIZE }, nodoTmp, mSM);
                block->setScale(Vector3(propX, propY, propZ));
                addBlock(block);
            }
            // Wall block
            else if (cell == WALL_BLOCK) {
                SceneNode* nodoTmp = SN->createChildSceneNode();
                block = new Wall({ iCol * BLOCK_SIZE, 0, iRow * BLOCK_SIZE }, nodoTmp, mSM, "cube.mesh");
                block->setScale(Vector3(propX, propY, propZ));
                addBlock(block);
            }
            // Wrong type of block
            else {

            }
            iCol++;
        }
        iRow++;
    }
    stageFile.close();
}