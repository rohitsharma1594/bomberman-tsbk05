#include <SDL.h>
#include <freeglut.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include "cube.h"
#include "dummyunbreakableobject.h"
#include "dummybreakableobject.h"
#include "bombobject.h"
#include "explosionobject.h"
#include "mapsubset.h"


using namespace std;

bool keepRunning = true;
const int size[2] = {800, 600};
//const GLfloat PI = 3.1415;

Model* cubeModel = new Model();
DumbObject cube(cubeModel, 0, 0, 0, 0);

const int levelWidth = 10, levelHeight = 10;
MapSubset<UnbreakableObject*, levelWidth, levelHeight> unbreakableWalls;
MapSubset<BreakableObject*, levelWidth, levelHeight> breakableWalls;
MapSubset<BombObject*, levelWidth, levelHeight> bombs;
MapSubset<ExplosionObject*, levelWidth, levelHeight> explosions;
MapSubset<bool, levelWidth, levelHeight> spawns;


// Load some kind of level for us to play, maybe a fixed/random level is fine for now
void loadLevel() {
	// Clear all things to make sure we start from scratch!
	{
		vector<MapSubsetStruct<UnbreakableObject*>*>::iterator begin = unbreakableWalls.getObjectVector()->begin(), end = unbreakableWalls.getObjectVector()->end();
		while (begin != end) {
			delete (*begin)->object;
			begin++;
		}
		unbreakableWalls.clear();
	}
	{
		vector<MapSubsetStruct<BreakableObject*>*>::iterator begin = breakableWalls.getObjectVector()->begin(), end = breakableWalls.getObjectVector()->end();
		while (begin != end) {
			delete (*begin)->object;
			begin++;
		}
		breakableWalls.clear();
	}
	{
		vector<MapSubsetStruct<BombObject*>*>::iterator begin = bombs.getObjectVector()->begin(), end = bombs.getObjectVector()->end();
		while (begin != end) {
			delete (*begin)->object;
			begin++;
		}
		bombs.clear();
	}
	{
		vector<MapSubsetStruct<ExplosionObject*>*>::iterator begin = explosions.getObjectVector()->begin(), end = explosions.getObjectVector()->end();
		while (begin != end) {
			delete (*begin)->object;
			begin++;
		}
		explosions.clear();
	}
	{
		spawns.clear();
	}


	// Set up the level, unbreakable walls all around, destroyable blocks everywhere else
	unbreakableWalls.insertAt(new UnbreakableObject(cubeModel), 0, 0);
	unbreakableWalls.insertAt(new UnbreakableObject(cubeModel), 0, 9);
	unbreakableWalls.insertAt(new UnbreakableObject(cubeModel), 9, 0);
	unbreakableWalls.insertAt(new UnbreakableObject(cubeModel), 9, 9);
	for (int i = 1; i < 9; i++) {
		unbreakableWalls.insertAt(new UnbreakableObject(cubeModel), i, 0);
		unbreakableWalls.insertAt(new UnbreakableObject(cubeModel), i, 9);
		unbreakableWalls.insertAt(new UnbreakableObject(cubeModel), 0, i);
		unbreakableWalls.insertAt(new UnbreakableObject(cubeModel), 9, i);
		for (int j = 1; j < 9; j++) {
			breakableWalls.insertAt(new BreakableObject(cubeModel), i, j);
		}
	}

	// Clear 4 corners for spawns
	delete breakableWalls.removeAt(1, 1);
	delete breakableWalls.removeAt(8, 1);
	delete breakableWalls.removeAt(1, 8);
	delete breakableWalls.removeAt(8, 8);
	spawns.insertAt(true, 1, 1);
	spawns.insertAt(true, 8, 1);
	spawns.insertAt(true, 1, 8);
	spawns.insertAt(true, 8, 8);
}


void reshape(GLsizei w, GLsizei h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1, 1,  -1, 1,  1, 20); // left, right, bottom, top, near, far
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 2.2,  0, 0, 0,  0, 1, 0); // camera, look-at-point, up-vector
	glPushMatrix(); 
}



static void doEvents() {
	SDL_Event event;

	while(SDL_PollEvent(&event)) {
		switch(event.type) {
		case SDL_VIDEORESIZE:
			cout << "video resize" << endl;
			reshape(event.resize.w, event.resize.h);
			break;
		case SDL_KEYDOWN:
			cout << "keydown: " << event.key.keysym.sym << endl;

			switch (event.key.keysym.sym) {
		case SDLK_ESCAPE:
			keepRunning = false;
			break;
			}
			break;
		case SDL_KEYUP:
			cout << "keyup: " << event.key.keysym.sym << endl;
			break;
		case SDL_QUIT:
			keepRunning = false;
			break;
		}

	}

}


void doDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*
	glPushMatrix();
	glRotatef(rotation, 0, 5, 1);
	glTranslatef(1.0, 0.0, 0.0);
	glRotatef(-rotation, 0, 5, 1);
	glScalef(0.2, 0.2, 0.2);
	glRotatef(rotation/2, 1, 1, 1);
	*/
	cube.draw();

	/*
	glPopMatrix();
	glPushMatrix();
	glRotatef(rotation/10, 1, 1, 1);
	glColor3f(1.0, 0.0, 1.0);
	*/
	//glutSolidSphere(0.3, 100, 100);

	//glPopMatrix();
	SDL_GL_SwapBuffers();
}


void doGameUpdate(float updateTime) {
	if (cube.object.getLocationX() > 1) {
		cube.object.addLocationX(-1);
	}
	cube.object.addLocationX((GLfloat)0.5*updateTime);
	cube.object.addRotation(10*updateTime);
}


int main(int argc, char *argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_SetVideoMode(size[0], size[1], 0, SDL_OPENGL | SDL_HWSURFACE | SDL_RESIZABLE);

	glClearColor((GLfloat)0.8, (GLfloat)1.0, (GLfloat)1.0, (GLfloat)1);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	reshape(size[0], size[1]);

	loadLevel();

	clock_t lastTime = clock();
	while(keepRunning) {
		doEvents();	//Process incoming events.

		float updateTime = (float)(clock() - lastTime) / CLOCKS_PER_SEC;
		lastTime = clock();

		doGameUpdate(updateTime);	//Update
		doDisplay();	//Draw the screen.
	}

	SDL_Quit();

	return 0;
}
