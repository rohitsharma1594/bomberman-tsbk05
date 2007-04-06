#include "playerobject.h"


PlayerObject::PlayerObject(Model* model, GLfloat x, GLfloat z, GLfloat y, GLfloat rotation, GLfloat scale) : Object(model, x, z, y, rotation, scale), lastBombDrop(-100), isDeadVar(false) {}


bool PlayerObject::canDropBomb(float currentTime) {
    return currentTime - lastBombDrop > (float)bombFrequency;
}


void PlayerObject::droppedBomb(float currentTime) {
    lastBombDrop = currentTime;
}