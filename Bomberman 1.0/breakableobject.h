#include "object.h"

#ifndef BREAKABLEOBJECT_H
#define BREAKABLEOBJECT_H

class BreakableObject : public Object {
public:
	BreakableObject(Model*, GLfloat, GLfloat, GLfloat = 0, GLfloat = 0, GLfloat = 1); 
};

#endif