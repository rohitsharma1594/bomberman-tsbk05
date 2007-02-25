#include "object.h"
#include <math.h>
#include <freeglut.h>


Object::Object(Model* model, GLfloat x = 0, GLfloat y = 0, GLfloat z = 0, GLfloat rotation = 0) : model(model), x(x), y(y), z(z), rotation(rotation) {
	//PI = acos((float)-1);
}


Object::~Object() {
}


void Object::draw() {
	glPushMatrix();
	glTranslatef(x, y, z);
	//glScalef(0.2, 0.2, 0.2);
	glRotatef(rotation, 0, 1, 0);

	model->draw();

	glPopMatrix();
}


void Object::addRotation(GLfloat newRotation) {
	rotation += newRotation;
}


void Object::addLocation(GLfloat newX, GLfloat newY, GLfloat newZ) {
	x += newX;
	y += newY;
	z += newZ;
}


void Object::setRotation(GLfloat newRotation) {
	rotation = newRotation;
}


void Object::setLocation(GLfloat newX, GLfloat newY, GLfloat newZ) {
	x = newX;
	y = newY;
	z = newX;
}