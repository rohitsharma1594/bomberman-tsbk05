#ifndef MODELLOADER_H
#define MODELLOADER_H

#include "model.h"

// A model should always be 1 wide, 1 long, with it's center(0, 0, 0) 0.5 above the ground
class ModelLoader : public Model {
	GLuint listId;
public:
	ModelLoader(const char*);
	void draw();
};

#endif

