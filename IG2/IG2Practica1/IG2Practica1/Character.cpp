#include "Character.h"
const int Character::SPEED = 500;

bool Character::isDirectionModified() {
    return getGridOrientation() != getNexDirVector();
}

void Character::changeDirection(tDir dir)
{
    sinbadDirectorion = dir;
}

Vector3 Character::getNexDirVector() {

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

Quaternion Character::getQuaternionForNewDirection() {

    Vector3 newDirVector = getNexDirVector();
    Quaternion q = getOrientation().getRotationTo(newDirVector);
    return q;
}

int Character::getSpeed()
{
    return SPEED;
}

void Character::rotateToNewDirection()
{
    if (!isDirectionModified())
        move(getNexDirVector() * SPEED /* evt.timeSinceLastFrame*/);
    else
        rotate(getQuaternionForNewDirection());
}

bool Character::is180Turn()
{
    return sinbadDirectorion == UP && getOrientation() == Vector3::UNIT_Z ||
        sinbadDirectorion == DOWN && getOrientation() == Vector3::NEGATIVE_UNIT_Z ||
        sinbadDirectorion == LEFT && getOrientation() == Vector3::UNIT_X ||
        sinbadDirectorion == RIGHT && getOrientation() == Vector3::NEGATIVE_UNIT_X;

}