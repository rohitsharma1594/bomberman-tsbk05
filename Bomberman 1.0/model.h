#ifndef MODEL_H
#define MODEL_H

#include "freeglut.h"

// A model should always be 1 wide, 1 long, with it's center(0, 0, 0) 0.5 above the ground
class Model {
	GLfloat scale;
public:
	Model(GLfloat = 1);
	void draw();
};

#endif