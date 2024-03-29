#include "bombobject.h"


BombObject::BombObject(Mix_Chunk *explosionSound, float createTime, Model* model, GLfloat x, GLfloat z, GLfloat y, GLfloat rotation, GLfloat scale) : Object(model, x, z, y, rotation, scale), explodeTime(createTime + 2), explosionSound(explosionSound), originalScale(scale) {}


bool BombObject::isExploded(float currentTime) {
    return currentTime >= explodeTime;
}


Mix_Chunk* BombObject::getSound() {
	return explosionSound;
}


void BombObject::updateScale(float currentTime) {
	setScale(originalScale + originalScale/(1+explodeTime-currentTime));
}