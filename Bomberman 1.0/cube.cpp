#include "cube.h"


DumbObject::DumbObject(Model* model, GLfloat x = 0, GLfloat y = 0, GLfloat z = 0, GLfloat rotation = 0) : object(model, x, y, z, rotation) {
	return;
}


void DumbObject::draw() {
	object.draw();
}
