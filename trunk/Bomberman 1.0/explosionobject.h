#include "object.h"

#ifndef EXPLOSIONOBJECT_H
#define EXPLOSIONOBJECT_H

class ExplosionObject : public Object {
public:
	ExplosionObject(Model*, GLfloat, GLfloat, GLfloat = 0, GLfloat = 0, GLfloat = 1); 
};

#endif

