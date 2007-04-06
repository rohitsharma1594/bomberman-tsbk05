#include "bombobject.h"


BombObject::BombObject(float createTime, Model* model, GLfloat x, GLfloat z, GLfloat y, GLfloat rotation, GLfloat scale) : Object(model, x, z, y, rotation, scale), explodeTime(createTime + 3) {}


bool BombObject::isExploded(float currentTime) {
    return currentTime >= explodeTime;
}


