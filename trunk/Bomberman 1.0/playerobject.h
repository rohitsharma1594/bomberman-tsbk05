#include "object.h"

#ifndef PLAYEROBJECT_H
#define PLAYEROBJECT_H

class PlayerObject : public Object {
    static const int bombFrequency = 0;
    float lastBombDrop;
    bool isDeadVar;
	int light;
public:
    PlayerObject(Model*, GLfloat, GLfloat, GLfloat = 0, GLfloat = 0, GLfloat = 1, int = -1);
    bool canDropBomb(float currentTime);
    void droppedBomb(float currentTime);
    bool isDead() {
        return isDeadVar;
    };
    void die() {
        isDeadVar = true;
    };
	void draw();
};

#endif
