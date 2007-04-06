#include "object.h"

#ifndef EXPLOSIONOBJECT_H
#define EXPLOSIONOBJECT_H

class ExplosionObject : public Object {
    float endTime;
public:
    ExplosionObject(float, Model*, GLfloat, GLfloat, GLfloat = 0, GLfloat = 0, GLfloat = 1);
    bool isFaded(float currentTime);
};

#endif

