#include "Character.h"
const int Character::SPEED = 150;

Vector3 Character::dirToVector(tDir dir) {
    switch (dir) {
    case RIGHT: return Vector3::UNIT_X;
    case LEFT:  return Vector3::NEGATIVE_UNIT_X;
    case DOWN:  return Vector3::UNIT_Z;
    case UP:    return Vector3::NEGATIVE_UNIT_Z;
    }
    return Vector3::ZERO;
}

Character::tDir Character::opposite(tDir dir) {
    switch (dir) {
    case UP:    return DOWN;
    case DOWN:  return UP;
    case LEFT:  return RIGHT;
    default:    return LEFT;
    }
}

bool Character::isDirectionModified() {
    return getGridOrientation() != getNexDirVector();
}

void Character::changeDirection(tDir dir)
{
    sinbadDirectorion = dir;
}

Vector3 Character::getNexDirVector() {
    return dirToVector(sinbadDirectorion);
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
    if (isDirectionModified())
        rotate(getQuaternionForNewDirection());
}

bool Character::is180Turn()
{
    return sinbadDirectorion == UP && getOrientation() == Vector3::UNIT_Z ||
        sinbadDirectorion == DOWN && getOrientation() == Vector3::NEGATIVE_UNIT_Z ||
        sinbadDirectorion == LEFT && getOrientation() == Vector3::UNIT_X ||
        sinbadDirectorion == RIGHT && getOrientation() == Vector3::NEGATIVE_UNIT_X;

}