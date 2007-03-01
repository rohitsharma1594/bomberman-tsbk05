#include "object.h"

#ifndef PLAYEROBJECT_H
#define PLAYEROBJECT_H

class PlayerObject : public Object {
public:
	PlayerObject(Model*, GLfloat, GLfloat, GLfloat = 0, GLfloat = 0, GLfloat = 1);
};

#endif