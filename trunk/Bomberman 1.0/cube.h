#ifndef CUBE_H
#define CUBE_H

#include "object.h"


class DumbObject {
public:
	Object object;
	DumbObject(Model*, GLfloat, GLfloat, GLfloat, GLfloat);

	void draw();
};



#endif
