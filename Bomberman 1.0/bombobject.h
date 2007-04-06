#include "object.h"

#ifndef BOMBOBJECT_H
#define BOMBOBJECT_H

class BombObject : public Object {
    float explodeTime;
public:
    BombObject(float, Model*, GLfloat, GLfloat, GLfloat = 0, GLfloat = 0, GLfloat = 1);
    bool isExploded(float updateTime);
};

#endif
