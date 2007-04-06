#include "explosionobject.h"


ExplosionObject::ExplosionObject(float endTime, Model* model, GLfloat x, GLfloat z, GLfloat y, GLfloat rotation, GLfloat scale) : Object(model, x, z, y, rotation, scale), endTime(endTime) {}


bool ExplosionObject::isFaded(float currentTime) {
    return currentTime >= endTime;
}
