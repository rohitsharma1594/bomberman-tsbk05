#include "model.h"

#ifndef OBJECT_H
#define OBJECT_H


class Object {
	//static GLfloat PI;  // Borde vara static const men jag vette fan hur :)
	GLfloat x, y, z, rotation;
	Model* model;
public:
	Object(Model*, GLfloat, GLfloat, GLfloat, GLfloat);
	void draw();
	void addRotation(GLfloat);
	void addLocation(GLfloat, GLfloat, GLfloat);
	void addLocationX(GLfloat newX) { x += newX; };
	void addLocationY(GLfloat newY) { y += newY; };
	void addLocationZ(GLfloat newZ) { z += newZ; };
	void setRotation(GLfloat);
	void setLocation(GLfloat, GLfloat, GLfloat);
	void setLocationX(GLfloat newX) { x = newX; };
	void setLocationY(GLfloat newY) { y = newY; };
	void setLocationZ(GLfloat newZ) { z = newZ; };


	GLfloat getLocationX() { return x; };
	GLfloat getLocationY() { return y; };
	GLfloat getLocationZ() { return z; };
	GLfloat getRotation() { return rotation; };
};

#endif