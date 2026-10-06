#include "Labyrinth.h"

void Labyrinth::addBlock(Block* block, int row, int col, Vector3 tam)
{
    block->setScale(tam);

    blocks[row][col] = block;
}

Block* Labyrinth::getBlock(Vector3 position)
{
    int x = round(position.x / BLOCK_SIZE);
    int z = round(position.z / BLOCK_SIZE);
    return blocks[z][x];
}

void Labyrinth::createLabyrinth(std::string stageFileName, SceneManager* SM, Simbad* heroe)
{
    mSM = SM;
    SN = mSM->getRootSceneNode()->createChildSceneNode();

    stageFile.open(stageFileName);
    // Read the number of files and columns
    stageFile >> numRows;
    stageFile >> numCols;

    blocks.resize(numCols, std::vector<Block*>(numRows));

    bool ok = true;
    int iRow = 0;
    int iCol = 0;

    auto newNode = [this]() {
        SceneNode* n = SN->createChildSceneNode();
        n->showBoundingBox(true);
        return n;
        };

    Wall* referencia = new Wall(Vector3::ZERO, newNode(), mSM, "cube.mesh");
    Vector3 tam = referencia->calculateBoxSize();
    float propX = BLOCK_SIZE / tam.x;
    float propY = BLOCK_SIZE / tam.y;
    float propZ = BLOCK_SIZE / tam.z;
    Vector3 newTam(propX, propY, propZ);
    delete referencia;


    while (iRow < numRows && ok) {
        iCol = 0;
        while (iCol < numCols && ok) {
            stageFile >> cell;
            // Inserts an empty block!
            if (cell == EMPTY_BLOCK) {
                addBlock(new Empty({ iCol * BLOCK_SIZE, 0, iRow * BLOCK_SIZE }, newNode(), mSM), iRow, iCol, newTam);
            }
            // Wall block
            else if (cell == WALL_BLOCK) {
                addBlock(new Wall({ iCol * BLOCK_SIZE, 0, iRow * BLOCK_SIZE }, newNode(), mSM, "cube.mesh"), iRow, iCol, newTam);
            }
            // Invisible block
            else if (cell == INVISIBLE_BLOCK) {
                addBlock(new InvisibleBlock({ iCol * BLOCK_SIZE, 0, iRow * BLOCK_SIZE }, newNode(), mSM, "cube.mesh"), iRow, iCol, newTam);
            }
            // Fake block
            else if (cell == FAKE_BLOCK) {
                addBlock(new FakeBlock({ iCol * BLOCK_SIZE, 0, iRow * BLOCK_SIZE }, newNode(), mSM, "cube.mesh"), iRow, iCol, newTam);
            }
            // Breakable block
            else if (cell == BREAKABLE_BLOCK) {
                addBlock(new BreakableBlock({ iCol * BLOCK_SIZE, 0, iRow * BLOCK_SIZE }, newNode(), mSM, "cube.mesh"), iRow, iCol, newTam);
            }
            // Hero position
            else if (cell == HERO_CELL) {
                heroe->setPosition({ iCol * BLOCK_SIZE, 0, iRow * BLOCK_SIZE });
                addBlock(new Empty({ iCol * BLOCK_SIZE, 0, iRow * BLOCK_SIZE }, newNode(), mSM), iRow, iCol, newTam);
            }
            // Wrong type of block


            iCol++;
        }
        iRow++;
    }
    stageFile.close();
}

void Labyrinth::moveCharacter(Character* character, Real time) {
    Block* charBlock, * inFrontBlock;

    // Get the block where the character is placed, and the next one
    charBlock = this->getBlock(character->getPosition());

    inFrontBlock = this->getBlock(
        charBlock->getPosition() + character->getGridOrientation() * BLOCK_SIZE
    );
    // Character does not change its direction -> step forward!
    if (!character->isDirectionModified()) {
        stepForward(character, charBlock, inFrontBlock, time);
    }
    // New direction
    else {
        // Check the block in front of the character for the new direction
        Block* newDirBlock = this->getBlock(
            charBlock->getPosition() + character->getNexDirVector() * BLOCK_SIZE
        );

        // New position of the character after moving... (for checking if the center of the block is reached)
        Vector3 charNewPos = character->getPosition() +
            character->getGridOrientation() * character->getSpeed() * time;

        Vector3 difference(
            charNewPos.x - charBlock->getPosition().x,
            0,
            charNewPos.z - charBlock->getPosition().z
        );

        float tolerance = character->getSpeed() * time;

        // Check if the character can rotate for a new VALID direction
        if (newDirBlock->canPassThrough() &&
            blockCenterReached(difference, character->getGridOrientation(), tolerance)) {
            character->setPosition(charBlock->getPosition());
            character->rotateToNewDirection();
        }
        // 180 turn?
        else if (character->is180Turn()) {
            character->rotateToNewDirection();
        }
        // Rotation cannot be performed... check if character can step forward
        else {
            stepForward(character, charBlock, inFrontBlock, time);
        }
    }
}

void Labyrinth::stepForward(Character* character, Block* charBlock, Block* inFrontBlock, Real time)
{
    Vector3 direction = character->getGridOrientation();
    float step = character->getSpeed() * time;

    if (inFrontBlock->canPassThrough()) {
        character->move(direction * step);
        return;
    }

    Vector3 position = character->getPosition();
    Vector3 center = charBlock->getPosition();
    float remaining = (center - position).dotProduct(direction);

    if (remaining > 0.0f) {
        if (step >= remaining)
            character->setPosition(center);
        else
            character->move(direction * step);
    }
}

bool Labyrinth::blockCenterReached(Vector3 difference, Vector3 direction, float tolerance)
{
    return Ogre::Math::Abs(difference.x) <= tolerance &&
        Ogre::Math::Abs(difference.z) <= tolerance;
}

void Labyrinth::update()
{
    for (auto& fila : blocks)
        for (Block* b : fila)
            b->update();
}