#include "model.h"

#ifndef OBJECT_H
#define OBJECT_H


class Object {
    GLfloat x, y, z, rotation, scale;
    //GLfloat crx, cry, crz;
    //GLfloat crxInc, cryInc, crzInc;

    Model* model;
public:
    Object(Model*, GLfloat, GLfloat, GLfloat = 0, GLfloat = 0, GLfloat = 1);
    ~Object();
    void draw();
    void addRotation(GLfloat);
    void addConstantRotation(GLfloat, GLfloat, GLfloat);
    void addLocation(GLfloat, GLfloat, GLfloat);
    void addLocationX(GLfloat newX) {
        x += newX;
    };
    void addLocationY(GLfloat newY) {
        y += newY;
    };
    void addLocationZ(GLfloat newZ) {
        z += newZ;
    };
    void setRotation(GLfloat);
    void setLocation(GLfloat, GLfloat, GLfloat);
    void setLocationX(GLfloat newX) {
        x = newX;
    };
    void setLocationY(GLfloat newY) {
        y = newY;
    };
    void setLocationZ(GLfloat newZ) {
        z = newZ;
    };
	void setScale(GLfloat newScale) {
		scale = newScale;
	}


    GLfloat getLocationX() {
        return x;
    };
    GLfloat getLocationY() {
        return y;
    };
    GLfloat getLocationZ() {
        return z;
    };
    GLfloat getRotation() {
        return rotation;
    };
	GLfloat getScale() {
		return scale;
	}
};

#endif
