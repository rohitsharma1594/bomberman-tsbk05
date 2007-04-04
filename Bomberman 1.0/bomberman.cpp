#include <SDL.h>
#include <freeglut.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include "unbreakableobject.h"
#include "breakableobject.h"
#include "bombobject.h"
#include "explosionobject.h"
#include "mapsubset.h"
#include "playerobject.h"
#include "maputilities.h"
#include "cube2.h"
#include "modelloader.h"


using namespace std;

bool keepRunning = true;
const int size[2] =
    {
        800, 600
    };
const GLfloat playerSpeed = 3;  // The speed of our player in blocks/second

// Model* unbreakableModel = new ModelLoader("bart.3DS");
// Model* breakableModel = new Cube2();
// Model* playerModel = new Cube2();
bool keyboard[400]; // The SDL_-keywords are defined all the way up to 322'ish, this works for now

int levelWidth = 1;
int levelHeight = 1;

MapSubset<UnbreakableObject*>* unbreakableWalls;
MapSubset<BreakableObject*>* breakableWalls;
MapSubset<BombObject*>* bombs;
MapSubset<ExplosionObject*>* explosions;
MapSubset<bool>* spawns;
PlayerObject* player1;

// Load some kind of level for us to play, maybe a fixed/random level is fine for now
void loadLevel()
{

    // Clear all things to make sure we start from scratch!
    {
        vector<MapSubsetStruct<UnbreakableObject*>*>::iterator 
				begin = unbreakableWalls->getObjectVector()->begin(), 
				end = unbreakableWalls->getObjectVector()->end();
        
				for (;begin != end; begin++)
        {
            delete (*begin)->object;
            begin++;
        }
        unbreakableWalls->clear();
    }
		
    {
    	vector<MapSubsetStruct<BreakableObject*>*>::iterator 
				begin = breakableWalls->getObjectVector()->begin(), 
				end = breakableWalls->getObjectVector()->end();
    	for (;begin != end; begin++) {
    		delete (*begin)->object;
    		begin++;
    	}
    	breakableWalls->clear();
    }
    {
    	vector<MapSubsetStruct<BombObject*>*>::iterator 
				begin = bombs->getObjectVector()->begin(), 
				end = bombs->getObjectVector()->end();
    	for (;begin != end; begin++) {
    		delete (*begin)->object;
    		begin++;
    	}
    	bombs->clear();
    }
    {
    	vector<MapSubsetStruct<ExplosionObject*>*>::iterator 
				begin = explosions->getObjectVector()->begin(), 
				end = explosions->getObjectVector()->end();
    	for (;begin != end; begin++) {
    		delete (*begin)->object;
    		begin++;
    	}
    	explosions->clear();
    }
    {
    	spawns->clear();
    }

    /*
    // Set up the level, unbreakable walls all around, destroyable blocks everywhere else
    unbreakableWalls.insertAt(new UnbreakableObject(unbreakableModel, 0, 0), 0, 0);
    unbreakableWalls.insertAt(new UnbreakableObject(unbreakableModel, 0, 9), 0, 9);
    unbreakableWalls.insertAt(new UnbreakableObject(unbreakableModel, 9, 0), 9, 0);
    unbreakableWalls.insertAt(new UnbreakableObject(unbreakableModel, 9, 9), 9, 9);
    for (int i = 1; i < 9; i++) {
    	unbreakableWalls.insertAt(new UnbreakableObject(unbreakableModel, i, 0), i, 0);
    	unbreakableWalls.insertAt(new UnbreakableObject(unbreakableModel, i, 9), i, 9);
    	unbreakableWalls.insertAt(new UnbreakableObject(unbreakableModel, 0, i), 0, i);
    	unbreakableWalls.insertAt(new UnbreakableObject(unbreakableModel, 9, i), 9, i);
    	for (int j = 1; j < 9; j++) {
    		breakableWalls.insertAt(new BreakableObject(breakableModel, i, j), i, j);
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
    */

		// Load unbreakable objects i.e walls
		assert(MapUtilities<UnbreakableObject>::readMap("maze.data", unbreakableWalls, new ModelLoader("cube_frame_subsurf_2.3ds"),0.5, '#'));
		
    // Place a player!
    if (player1 != NULL)
    {
        delete player1;
    }
    player1 = new PlayerObject(new ModelLoader("lolly.3DS"), 1, 1, 0, 180.0, 0.01);
		player1->addConstantRotation( 10,10,10);
}


void reshape(GLsizei w, GLsizei h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1, 1,  -1, 1,  1, 20); // left, right, bottom, top, near, far
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(4.5, 5, -2.2,  4.5, 3, 0,  0, 1, 0); // camera, look-at-point, up-vector
    glPushMatrix();
}

static void doEvents()
{
    SDL_Event event;

    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
        case SDL_VIDEORESIZE:
            cout << "video resize" << endl;
            reshape(event.resize.w, event.resize.h);
            break;
        case SDL_KEYDOWN:
            cout << "keydown: " << event.key.keysym.sym << endl;
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                keepRunning = false;
                break;
            }
            cout << "keydown: " << event.key.keysym.sym << endl;
            if (event.key.keysym.sym < 400)
            { // We don't want to go outside our array
                keyboard[event.key.keysym.sym] = true;
            }
            break;
        case SDL_KEYUP:
            cout << "keyup: " << event.key.keysym.sym << endl;
            if (event.key.keysym.sym < 400)
            { // We don't want to go outside our array
                keyboard[event.key.keysym.sym] = false;
            }
            break;
        case SDL_QUIT:
            keepRunning = false;
            break;
        }
    }

}


void doDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*
    glPushMatrix();
    glRotatef(rotation, 0, 5, 1);
    glTranslatef(1.0, 0.0, 0.0);
    glRotatef(-rotation, 0, 5, 1);
    glScalef(0.2, 0.2, 0.2);
    glRotatef(rotation/2, 1, 1, 1);
    */
    //cube.draw();

    /*
    glPopMatrix();
    glPushMatrix();
    glRotatef(rotation/10, 1, 1, 1);
    glColor3f(1.0, 0.0, 1.0);
    */
    //glutSolidSphere(0.3, 100, 100);

    //glPopMatrix();

    {
        vector<MapSubsetStruct<UnbreakableObject*>*>::iterator 
					begin = unbreakableWalls->getObjectVector()->begin(), 
					end = unbreakableWalls->getObjectVector()->end();
        for (;begin != end; begin++)
        {
            (*begin)->object->draw();
        }
    }
    
    {
    	vector<MapSubsetStruct<BreakableObject*>*>::iterator 
				begin = breakableWalls->getObjectVector()->begin(), 
				end = breakableWalls->getObjectVector()->end();
    	for (;begin != end; begin++) {
    		(*begin)->object->draw();
    	}
    }
    {
    	vector<MapSubsetStruct<BombObject*>*>::iterator 
				begin = bombs->getObjectVector()->begin(), 
				end = bombs->getObjectVector()->end();
    	for (;begin != end; begin++) {
    		(*begin)->object->draw();
    	}
    }
    {
    	vector<MapSubsetStruct<ExplosionObject*>*>::iterator 
				begin = explosions->getObjectVector()->begin(), 
				end = explosions->getObjectVector()->end();
    	for (;begin != end; begin++) {
    		(*begin)->object->draw();
    	}
    }

    player1->draw();

    SDL_GL_SwapBuffers();
}


void doGameUpdate(float updateTime)
{
    if (keyboard[SDLK_LEFT] && !keyboard[SDLK_RIGHT] && !keyboard[SDLK_UP] && !keyboard[SDLK_DOWN])
    {
        player1->addLocationX(playerSpeed*updateTime);
    }
    if (keyboard[SDLK_LEFT] && !keyboard[SDLK_RIGHT] && keyboard[SDLK_UP] && !keyboard[SDLK_DOWN])
    {
        player1->addLocationX(playerSpeed*updateTime);
        player1->addLocationZ(playerSpeed*updateTime);
    }
    if (!keyboard[SDLK_LEFT] && !keyboard[SDLK_RIGHT] && keyboard[SDLK_UP] && !keyboard[SDLK_DOWN])
    {
        player1->addLocationZ(playerSpeed*updateTime);
    }
    if (!keyboard[SDLK_LEFT] && keyboard[SDLK_RIGHT] && keyboard[SDLK_UP] && !keyboard[SDLK_DOWN])
    {
        player1->addLocationX(-playerSpeed*updateTime);
        player1->addLocationZ(playerSpeed*updateTime);
    }
    if (!keyboard[SDLK_LEFT] && keyboard[SDLK_RIGHT] && !keyboard[SDLK_UP] && !keyboard[SDLK_DOWN])
    {
        player1->addLocationX(-playerSpeed*updateTime);
    }
    if (!keyboard[SDLK_LEFT] && keyboard[SDLK_RIGHT] && !keyboard[SDLK_UP] && keyboard[SDLK_DOWN])
    {
        player1->addLocationX(-playerSpeed*updateTime);
        player1->addLocationZ(-playerSpeed*updateTime);
    }
    if (!keyboard[SDLK_LEFT] && !keyboard[SDLK_RIGHT] && !keyboard[SDLK_UP] && keyboard[SDLK_DOWN])
    {
        player1->addLocationZ(-playerSpeed*updateTime);
    }
    if (keyboard[SDLK_LEFT] && !keyboard[SDLK_RIGHT] && !keyboard[SDLK_UP] && keyboard[SDLK_DOWN])
    {
        player1->addLocationX(playerSpeed*updateTime);
        player1->addLocationZ(-playerSpeed*updateTime);
    }
}


int main(int argc, char *argv[])
{

    /* Init the subsets and set global map width / height */
    MapUtilities<UnbreakableObject>::getMapSize("maze.data", levelHeight, levelWidth);
    unbreakableWalls=new MapSubset<UnbreakableObject*>(levelWidth, levelHeight);
		breakableWalls = new MapSubset<BreakableObject*>(levelWidth, levelHeight);
		bombs = new MapSubset<BombObject*>(levelWidth, levelHeight);
		explosions = new MapSubset<ExplosionObject*>(levelWidth, levelHeight);
		spawns = new MapSubset<bool>(levelWidth, levelHeight);

		/* SDL INITIATION */
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_SetVideoMode(size[0], size[1], 0, SDL_OPENGL | SDL_HWSURFACE | SDL_RESIZABLE);

		/* OPENGL SETUP */
    glClearColor((GLfloat)0.8, (GLfloat)1.0, (GLfloat)1.0, (GLfloat)1);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_NORMALIZE);

    reshape(size[0], size[1]);

		/* GAME SETUP */
    loadLevel();

		/* MAIN LOOP */
    clock_t lastTime = clock(), currTime = clock();
    while(keepRunning)
    {
        doEvents();	//Process incoming events.

        currTime = clock();
        float updateTime = (float)(currTime - lastTime) / CLOCKS_PER_SEC;
        lastTime = currTime;
        doGameUpdate(updateTime);	//Update

        doDisplay();	//Draw the screen.
    }

    SDL_Quit();

    return 0;
}

