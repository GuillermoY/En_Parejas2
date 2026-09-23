#include "Labyrinth.h"

void Labyrinth::addBlock(Block* block, int row)
{
    blocks[row].push_back(block);
}

Block* Labyrinth::getBlock(Vector3 position)
{
    int x = position.x;
    int z = position.z;

    return blocks[x][z];
}

void Labyrinth::createLabyrinth(std::string stageFileName, SceneManager* SM, Simbad* heroe)
{
    mSM = SM;
    SN = mSM->getRootSceneNode()->createChildSceneNode();

    stageFile.open(stageFileName);
    // Read the number of files and columns
    stageFile >> numRows;
    stageFile >> numCols;

    blocks[numRows][numRows];

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
                nodoTmp->showBoundingBox(true);
                addBlock(block, iRow);
            }
            // Wall block
            else if (cell == WALL_BLOCK) {
                SceneNode* nodoTmp = SN->createChildSceneNode();
                block = new Wall({ iCol * BLOCK_SIZE, 0, iRow * BLOCK_SIZE }, nodoTmp, mSM, "cube.mesh");
                block->setScale(Vector3(propX, propY, propZ));
                nodoTmp->showBoundingBox(true);
                addBlock(block, iRow);
            }
            // Hero position
            else if (cell == HERO_CELL) {
                heroe->setPosition({ iCol * BLOCK_SIZE, 0, iRow * BLOCK_SIZE });
                SceneNode* nodoTmp = SN->createChildSceneNode();
                block = new Empty({ iCol * BLOCK_SIZE, 0, iRow * BLOCK_SIZE }, nodoTmp, mSM);
                block->setScale(Vector3(propX, propY, propZ));
                nodoTmp->showBoundingBox(true);
                addBlock(block, iRow);
            }
            // Wrong type of block

            iCol++;
        }
        iRow++;
    }
    stageFile.close();
}

//void Labyrinth::moveCharacter(Character* character, Real time) {
//    Block* charBlock, * inFrontBlock;
//    // Get the block where the character is placed, and the next one
//    charBlock = this->getBlock(character->getPosition());
//    inFrontBlock = this->getBlock((character->getGridOrientation() * BLOCK_SIZE) + character->getPosition());
//    // Character does not change its direction -> step forward!
//    if (!character->isDirectionModified())
//        stepForward(character, . . ., time);
//    // New direction
//    else {
//        // Check the block in front of the character for the new direction
//        Block* newDirBlock = this->getBlock(character->getPosition() + (character->getNexDirVector() * WallBlock::BLOCK_SIZE));
//        // New position of the character after moving... (for checking if the center of the block is reached)
//        Vector3 charNewPos = character->getPosition() + (character->getGridOrientation() * character->getSpeed() * time);
//        Vector3 difference = Vector3(charNewPos.x - charBlock->getPosition().x, 0, charNewPos.z - charBlock->getPosition().z);
//        // Check if the character can rotate for a new VALID direction
//        if (newDirBlock->canPassThrough() && blockCenterReached(difference, character->getGridOrientation()))
//            character->rotateToNewDirection();
//        // 180 turn?
//        else if (character->is180Turn())
//            character->rotateToNewDirection();
//        // Rotation cannot be performed... check if character can step forward
//        else
//            stepForward(character, . . ., time);
//    }
//}