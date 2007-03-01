#include "object.h"

#ifndef UNBREAKABLEOBJECT_H
#define UNBREAKABLEOBJECT_H

class UnbreakableObject : public Object {
public:
	UnbreakableObject(Model*, GLfloat, GLfloat, GLfloat = 0, GLfloat = 0, GLfloat = 1); 
};

#endif