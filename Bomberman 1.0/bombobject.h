#include "object.h"

#ifndef BOMBOBJECT_H
#define BOMBOBJECT_H

class BombObject : public Object {
public:
	BombObject(Model*, GLfloat, GLfloat, GLfloat = 0, GLfloat = 0, GLfloat = 1); 
};

#endif
