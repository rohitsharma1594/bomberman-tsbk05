#include "playerobject.h"
#include <freeglut.h>


PlayerObject::PlayerObject(Model* model, GLfloat x, GLfloat z, GLfloat y, GLfloat rotation, GLfloat scale, int light) : Object(model, x, z, y, rotation, scale), lastBombDrop(-100), isDeadVar(false), light(light) {}


bool PlayerObject::canDropBomb(float currentTime) {
    return currentTime - lastBombDrop > (float)bombFrequency;
}


void PlayerObject::droppedBomb(float currentTime) {
    lastBombDrop = currentTime;
}


void PlayerObject::draw() {
	if (light >= 0) {
		GLfloat position[4];
		position[0] = getLocationX();
		position[1] = 3;
		position[2] = getLocationZ();
		position[3] = 1;
		glLightfv(light, GL_POSITION, position);
	}
	((Object*)this)->draw();
}
