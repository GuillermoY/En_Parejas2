#include "Labyrinth.h"
#include <random>

int Labyrinth::floorCount = 0;

void Labyrinth::addBlock(Block* block, int row, int col, Vector3 tam, bool isWall)
{
    if (isWall)
        block->setMaterialName(wallMat);
    block->setScale(tam);
    blocks[row][col] = block;
}

void Labyrinth::addVillain(Villain* villain, int row, int col)
{
    villain->setPosition({ col * BLOCK_SIZE, 0, row * BLOCK_SIZE });
    villains.push_back(villain);
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
    hero = heroe;

    SN = mSM->getRootSceneNode()->createChildSceneNode();
    stageFile.open(stageFileName);
    // Read the number of files and columns
    stageFile >> numRows;
    stageFile >> numCols;

    stageFile >> wallMat;
    stageFile >> floorMat;

    stageFile >> lightType;

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
                addBlock(new Empty({ iCol * BLOCK_SIZE, 0, iRow * BLOCK_SIZE }, newNode(), mSM), iRow, iCol, newTam, false);
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
                addBlock(new Empty({ iCol * BLOCK_SIZE, 0, iRow * BLOCK_SIZE }, newNode(), mSM), iRow, iCol, newTam, false);
            }
            else if (cell == VILLAIN_CELL) {
                addVillain(new Villain({ iCol * BLOCK_SIZE, 0, iRow * BLOCK_SIZE }, newNode(), mSM, "ogrehead.mesh"), iRow, iCol);
                addBlock(new Empty({ iCol * BLOCK_SIZE, 0, iRow * BLOCK_SIZE }, newNode(), mSM), iRow, iCol, newTam, false);
            }
            else if (cell == OG_VILLAIN_CELL)
            {
                //Villain* v = new Villain({ iCol * BLOCK_SIZE, 0, iRow * BLOCK_SIZE }, newNode(), mSM, "springtrapHead.glb");
                OGVillain* v = new OGVillain({ iCol * BLOCK_SIZE, 0, iRow * BLOCK_SIZE }, newNode(), mSM, "springtrapHead.glb");
                v->setScale(Vector3(30, 30, 30));
                addVillain(v, iRow, iCol);
                addBlock(new Empty({ iCol * BLOCK_SIZE, 0, iRow * BLOCK_SIZE }, newNode(), mSM), iRow, iCol, newTam, false);
            }
            // Wrong type of block


            iCol++;
        }
        iRow++;
    }
    // The light


    Ogre::Light* spotLight1 = mSM->createLight("SpotLight1");
    Light::LightTypes type;
    if (lightType == "directional")
        type = Light::LightTypes::LT_DIRECTIONAL;
    else if (lightType == "spot")
        type = Light::LightTypes::LT_SPOTLIGHT;
    else
        type = Light::LightTypes::LT_POINT;
    spotLight1->setType(type);
    spotLight1->setSpotlightInnerAngle(Ogre::Degree(50.0f));
    spotLight1->setSpotlightOuterAngle(Ogre::Degree(50.0f));
    spotLight1->setSpotlightFalloff(0.0f);
    spotLight1->setDiffuseColour(1.0f, 1.0f, 1.0f);
    // Node with the light attached
    mLightNode = mSM->getRootSceneNode()->createChildSceneNode();
    mLightNode->setDirection(Ogre::Vector3(0, -1, 0));
    mLightNode->attachObject(spotLight1);
    mLightNode->setPosition({ heroe->getPosition().x ,
        heroe->getPosition().y +100,
        heroe->getPosition().z });

    createFloor();

    stageFile.close();
}

void Labyrinth::createFloor()
{
    float width = numCols * BLOCK_SIZE;
    float depth = numRows * BLOCK_SIZE;

    std::string meshName = "labyrinthFloor" + std::to_string(floorCount++);

    MeshManager::getSingleton().createPlane(meshName,
        ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        Plane(Vector3::UNIT_Y, 0),
        width, depth,
        numCols * 4, numRows * 4,
        true, 1,
        numCols, numRows,
        Vector3::UNIT_Z);

    Entity* floor = mSM->createEntity(meshName);
    floorNode = SN->createChildSceneNode();
    floorNode->attachObject(floor);
    floor->setMaterialName(floorMat);

    floorNode->setPosition((numCols - 1) * BLOCK_SIZE / 2.0f, -BLOCK_SIZE / 2.0f, (numRows - 1) * BLOCK_SIZE / 2.0f);
}

void Labyrinth::moveCharacter(Character* character, Real time) {
    Block* charBlock, * inFrontBlock;
    // Get the block where the character is placed, and the next one
    charBlock = this->getBlock(character->getPosition());

    inFrontBlock = this->getBlock(
        charBlock->getPosition() + character->getGridOrientation() * BLOCK_SIZE
    );
    bool move=false;
    // Character does not change its direction -> step forward!
    if (!character->isDirectionModified()) {
        move = stepForward(character, charBlock, inFrontBlock, time);
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
            move = stepForward(character, charBlock, inFrontBlock, time);
        }
    }
    if (move)
        character->setAnimState("RunBase");
    else
        character->setAnimState("IdleTop");
}

void Labyrinth::moveVillain(Villain* villain, Real time)
{
    Block* charBlock = getBlock(villain->getPosition());
    Vector3 center = charBlock->getPosition();
    float step = villain->getSpeed() * time;

    float remaining = (center - villain->getPosition()).dotProduct(villain->getGridOrientation());
    if (remaining >= 0.0f && remaining <= step)
        chooseVillainDirection(villain, charBlock);

    Block* inFrontBlock = getBlock(center + villain->getGridOrientation() * BLOCK_SIZE);
    stepForward(villain, charBlock, inFrontBlock, time);
}

void Labyrinth::checkVillainCollisions()
{
    for (Villain* v : villains)
        if (hero->getAABB().intersects(v->getAABB()))
            hero->hit();
}

void Labyrinth::chooseVillainDirection(Villain* villain, Block* charBlock)
{
    Vector3 center = charBlock->getPosition();
    Character::tDir current = villain->getDirectiorion();
    Character::tDir back = Character::opposite(current);

    std::vector<Character::tDir> options;
    bool canTurn90 = false;
    for (Character::tDir d : { Character::UP, Character::DOWN, Character::LEFT, Character::RIGHT }) {
        if (getBlock(center + Character::dirToVector(d) * BLOCK_SIZE)->canPassThrough()) {
            options.push_back(d);
            if (d != current && d != back) canTurn90 = true;
        }
    }

    bool blocked = !getBlock(center + Character::dirToVector(current) * BLOCK_SIZE)->canPassThrough();

    if (!blocked && !canTurn90)
        return;

    if (options.empty())
        return;

    std::uniform_int_distribution<size_t> dist(0, options.size() - 1);
    villain->setPosition(center);
    villain->changeDirection(options[dist(gen)]);
    villain->rotateToNewDirection();
}

bool Labyrinth::stepForward(Character* character, Block* charBlock, Block* inFrontBlock, Real time)
{
    Vector3 direction = character->getGridOrientation();
    float step = character->getSpeed() * time;

    if (inFrontBlock->canPassThrough()) {
        character->move(direction * step);
        return true;
    }

    Vector3 position = character->getPosition();
    Vector3 center = charBlock->getPosition();
    float remaining = (center - position).dotProduct(direction);

    if (remaining > 0.0f) {
        if (step >= remaining)
        {
            character->setPosition(center);
            return false;
        }
        else
        {
            character->move(direction * step);
        }
    }
    return false;
}


bool Labyrinth::blockCenterReached(Vector3 difference, Vector3 direction, float tolerance)
{
    return Ogre::Math::Abs(difference.x) <= tolerance &&
        Ogre::Math::Abs(difference.z) <= tolerance;
}

void Labyrinth::update(Real time)
{
    for (auto& fila : blocks)
        for (Block* b : fila)
            b->update();

    for (Villain* v : villains)
    {
        moveVillain(v, time);
        if (v->getOG())
            v->update(time);
    }


    hero->update(time);

    mLightNode->setPosition({ hero->getPosition().x,
    hero->getPosition().y +100,
    hero->getPosition().z });
    checkVillainCollisions();
}