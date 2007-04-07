#include "object.h"
#include <SDL_mixer.h>

#ifndef BOMBOBJECT_H
#define BOMBOBJECT_H

class BombObject : public Object {
    float explodeTime;
	Mix_Chunk *explosionSound;
public:
    BombObject(Mix_Chunk *explosionSound, float, Model*, GLfloat, GLfloat, GLfloat = 0, GLfloat = 0, GLfloat = 1);
    bool isExploded(float updateTime);
	Mix_Chunk* getSound();
};

#endif
