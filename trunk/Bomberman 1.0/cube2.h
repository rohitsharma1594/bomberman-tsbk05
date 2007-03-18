#ifndef CUBE2_H
#define CUBE2_H

#include "model.h"

// A model should always be 1 wide, 1 long, with it's center(0, 0, 0) 0.5 above the ground
class Cube2 : public Model {

public:
	Cube2();
	void draw();
};

#endif

