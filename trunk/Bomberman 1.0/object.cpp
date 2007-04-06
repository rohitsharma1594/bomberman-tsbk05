#include "object.h"
#include <math.h>
#include <freeglut.h>


Object::Object(Model* model, GLfloat x, GLfloat z, GLfloat y, GLfloat rotation, GLfloat scale) : model(model), x(x), z(z), y(y), rotation(rotation), scale(scale), crx(0), cry(0), crz(0) {}


Object::~Object() {}

void Object::draw() {
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(scale, scale, scale);
    glRotatef(rotation, 0, rotation, 0);
    glRotatef(rotation, crx, cry, crz);
    model->draw();
    glPopMatrix();

    crx+=crxInc;
    cry+=cryInc;
    crz+=crzInc;

    if (crx > 360)
        crx -= 360;
    if (cry > 360)
        cry -= 360;
    if (crz > 360)
        crz -= 360;
}

void Object::addRotation(GLfloat newRotation) {
    rotation += newRotation;
}

void Object::addConstantRotation(GLfloat rx, GLfloat ry, GLfloat rz) {

    crxInc = rx;
    cryInc = ry;
    crzInc = rz;
}


void Object::addLocation(GLfloat newX, GLfloat newY, GLfloat newZ) {
    x += newX;
    y += newY;
    z += newZ;
}


void Object::setRotation(GLfloat newRotation) {
    rotation = newRotation;
}


void Object::setLocation(GLfloat newX, GLfloat newZ, GLfloat newY) {
    x = newX;
    z = newX;
    y = newY;
}

