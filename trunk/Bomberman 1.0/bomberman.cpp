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
//#include <cmath>


using namespace std;

bool keepRunning = true;
const int size[2] = {
                        800, 600
                    };
const GLfloat playerSpeed = 3;  // The speed of our player in blocks/second

// Model* unbreakableModel = new ModelLoader("bart.3DS");
// Model* breakableModel = new Cube2();
// Model* playerModel = new Cube2();
bool keyboard[400]; // The SDL_-keywords are defined all the way up to 322'ish, this works for now
const float leeway = 0.2;

int levelWidth = 1;
int levelHeight = 1;

MapSubset<UnbreakableObject*>* unbreakableWalls;
MapSubset<BreakableObject*>* breakableWalls;
MapSubset<BombObject*>* bombs;
MapSubset<ExplosionObject*>* explosions;
MapSubset<bool>* spawns;
PlayerObject* player1;
PlayerObject* player2;
Model* bombModel = NULL;
Model* explosionModel = NULL;
static const int BOMBRADIUS = 3;
static const int EXPLOSIONLIFETIME = 2;


int round(float x) {
    return floor(x+0.5);
}


int antiRound(float x) {
    int roundx = round(x);
    if (roundx < x) {
        return roundx+1;
    } else {
        return roundx-1;
    }
}


bool isMiddle(float x) {
    if (fabs(round(x)-x) > leeway+0.01) {
        return true;
    } else {
        return false;
    }
}



// Load some kind of level for us to play, maybe a fixed/random level is fine for now
void loadLevel() {

    // Clear all things to make sure we start from scratch!
    {
        vector<MapSubsetStruct<UnbreakableObject*>*>::iterator
        begin = unbreakableWalls->getObjectVector()->begin(),
                end = unbreakableWalls->getObjectVector()->end();

        for (;begin != end; begin++) {
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

    // Load unbreakable objects i.e walls
    assert(MapUtilities<UnbreakableObject>::readMap("maze.data", unbreakableWalls, new ModelLoader("cube_frame_subsurf_2.3ds"),0.5, '#'));

    // Place a player!
    if (player1 != NULL) {
        delete player1;
    }
	Model* playerModel = new ModelLoader("lolly.3DS");
    player1 = new PlayerObject(playerModel, 1, 1, 0, 180.0, 0.01);
    player2 = new PlayerObject(playerModel, 8, 8, 0, 180.0, 0.01);
}


void reshape(GLsizei w, GLsizei h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1, 1,  -1, 1,  1, 20); // left, right, bottom, top, near, far
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(4.5, 5, -2.2,  4.5, 3, 0,  0, 1, 0); // camera, look-at-point, up-vector
    glPushMatrix();
}

static void doEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_VIDEORESIZE:
            cout << "video resize" << endl;
            reshape(event.resize.w, event.resize.h);
            break;
        case SDL_KEYDOWN:
            cout << "keydown: " << event.key.keysym.sym << endl;
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                keepRunning = false;
                break;
            }
            cout << "keydown: " << event.key.keysym.sym << endl;
            if (event.key.keysym.sym < 400) { // We don't want to go outside our array
                keyboard[event.key.keysym.sym] = true;
            }
            break;
        case SDL_KEYUP:
            cout << "keyup: " << event.key.keysym.sym << endl;
            if (event.key.keysym.sym < 400) { // We don't want to go outside our array
                keyboard[event.key.keysym.sym] = false;
            }
            break;
        case SDL_QUIT:
            keepRunning = false;
            break;
        }
    }

}


void doDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    {
        vector<MapSubsetStruct<UnbreakableObject*>*>::iterator
        begin = unbreakableWalls->getObjectVector()->begin(),
                end = unbreakableWalls->getObjectVector()->end();
        for (;begin != end; begin++) {
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

    if (!player1->isDead()) {
        player1->draw();
    }
    if (!player2->isDead()) {
        player2->draw();
    }

    SDL_GL_SwapBuffers();
}


bool isObjectAt(int x, int y) {
    if (unbreakableWalls->isObjectAt(x, y)) {
        return true;
    } else if (breakableWalls->isObjectAt(x, y)) {
        return true;
    }

    return false;
}


bool isBombAt(int x, int y) {
    return bombs->isObjectAt(x, y);
}


bool isExplosionAt(int x, int y) {
    return explosions->isObjectAt(x, y);
}



bool isValidPosition(float x, float y) {
    if (isMiddle(x)) {
        if (isMiddle(y)) {
            if (isObjectAt(round(x), round(y)) || isObjectAt(antiRound(x), round(y)) || isObjectAt(round(x), antiRound(y)) || isObjectAt(antiRound(x), antiRound(y))) {
                return false;
            }
        } else {
            if (isObjectAt(round(x), round(y)) || isObjectAt(antiRound(x), round(y))) {
                return false;
            }
        }
    } else {
        if (isMiddle(y)) {
            if (isObjectAt(round(x), round(y)) || isObjectAt(round(x), antiRound(y))) {
                return false;
            }
        } else {
            if (isObjectAt(round(x), round(y))) {
                return false;
            }
        }
    }

    return true;
}


bool died(PlayerObject *player) {
    float x = player->getLocationX();
    float y = player->getLocationZ();
    if (isMiddle(x)) {
        if (isMiddle(y)) {
            if (isExplosionAt(round(x), round(y)) || isExplosionAt(antiRound(x), round(y)) || isExplosionAt(round(x), antiRound(y)) || isExplosionAt(antiRound(x), antiRound(y))) {
                return true;
            }
        } else {
            if (isExplosionAt(round(x), round(y)) || isExplosionAt(antiRound(x), round(y))) {
                return true;
            }
        }
    } else {
        if (isMiddle(y)) {
            if (isExplosionAt(round(x), round(y)) || isExplosionAt(round(x), antiRound(y))) {
                return true;
            }
        } else {
            if (isExplosionAt(round(x), round(y))) {
                return true;
            }
        }
    }

    return false;
}


void explosionAt(int x, int z, float endTime);


bool tryPlaceExplosion(int x, int z, float endTime) {
    if (isValidPosition(x, z)) {
        if (isBombAt(x, z)) {
            bombs->removeAt(x, z);
            explosionAt(x, z, endTime);
        } else if (isExplosionAt(x, z)) {
            //if (explosions->getObjectAt(x, z)->getExplosionRadius() < spreadCount) {
            explosions->removeAt(x, z);
            explosions->insertAt(new ExplosionObject(endTime, explosionModel, x, z, 0, 180, 0.01), x, z);
            //}
        } else {
            explosions->insertAt(new ExplosionObject(endTime, explosionModel, x, z, 0, 180, 0.01), x, z);
        }

        return true;
    }

    return false;
}


void explosionAt(int x, int z, float endTime) {
    tryPlaceExplosion(x, z, endTime);
    if (tryPlaceExplosion(x, z-1, endTime))
        if (tryPlaceExplosion(x, z-2, endTime))
            tryPlaceExplosion(x, z-3, endTime);
    if (tryPlaceExplosion(x, z+1, endTime))
        if (tryPlaceExplosion(x, z+2, endTime))
            tryPlaceExplosion(x, z+3, endTime);
    if (tryPlaceExplosion(x-1, z, endTime))
        if (tryPlaceExplosion(x-2, z, endTime))
            tryPlaceExplosion(x-3, z, endTime);
    if (tryPlaceExplosion(x+1, z, endTime))
        if (tryPlaceExplosion(x+2, z, endTime))
            tryPlaceExplosion(x+3, z, endTime);
}


void doGameUpdateForPlayer(float updateTime, float currentTime, PlayerObject* player, int left, int right, int up, int down, int bomb) {
	if (!player->isDead()) {
		float x = player->getLocationX();
		float z = player->getLocationZ();
		float originalX = x;
		float originalZ = z;
		if (keyboard[left] && !keyboard[right]) {
			x += playerSpeed*updateTime;
			if (isValidPosition(x, z)) {
				originalX = x;
			} else {
				x = round(originalX);
				if (x > originalX && isValidPosition(x, z)) {
					originalX = x;
				} else {
					x = originalX;
				}
			}
		} else if (!keyboard[left] && keyboard[right]) {
			x -= playerSpeed*updateTime;
			if (isValidPosition(x, z)) {
				originalX = x;
			} else {
				x = round(originalX);
				if (x < originalX && isValidPosition(x, z)) {
					originalX = x;
				} else {
					x = originalX;
				}
			}
		}


		if (keyboard[up] && !keyboard[down]) {
			z += playerSpeed*updateTime;
			if (isValidPosition(x, z)) {
				originalZ = z;
			} else {
				z = round(originalZ);
				if (z > originalZ && isValidPosition(x, z)) {
					originalZ = z;
				} else {
					z = originalZ;
				}
			}
		} else if (!keyboard[up] && keyboard[down]) {
			z -= playerSpeed*updateTime;
			if (isValidPosition(x, z)) {
				originalZ = z;
			} else {
				z = round(originalZ);
				if (z < originalZ && isValidPosition(x, z)) {
					originalZ = z;
				} else {
					z = originalZ;
				}
			}
		}
		player->setLocationX(x);
		player->setLocationZ(z);


		if (keyboard[bomb] && player->canDropBomb(currentTime)) {
			if (!isBombAt(round(player->getLocationX()), round(player->getLocationZ()))) {
				bombs->insertAt(new BombObject(currentTime, bombModel, round(player->getLocationX()), round(player->getLocationZ()), 0, 0, 0.01), round(player->getLocationX()), round(player->getLocationZ()));
				printf("Dropped a bomb!\n");
				player->droppedBomb(currentTime);
			}
		}
	}
}


void doGameUpdate(float updateTime, float currentTime) {
	doGameUpdateForPlayer(updateTime, currentTime, player1, SDLK_LEFT, SDLK_RIGHT, SDLK_UP, SDLK_DOWN, SDLK_RSHIFT);
	doGameUpdateForPlayer(updateTime, currentTime, player2, SDLK_a, SDLK_d, SDLK_w, SDLK_s, SDLK_LSHIFT);

	
	{
        vector<BombObject*> toRemove;
        {
            vector<MapSubsetStruct<BombObject*>*>::iterator
            begin = bombs->getObjectVector()->begin(),
                    end = bombs->getObjectVector()->end();
            for (;begin != end; begin++) {
                if ((*begin)->object->isExploded(currentTime)) {
                    toRemove.push_back((*begin)->object);
                }
            }
        }

        {
            vector<BombObject*>::iterator
            begin = toRemove.begin(),
                    end = toRemove.end();
            for (;begin != end; begin++) {
                bombs->removeAt((*begin)->getLocationX(), (*begin)->getLocationZ());
                explosionAt((*begin)->getLocationX(), (*begin)->getLocationZ(), currentTime + EXPLOSIONLIFETIME);
                delete *begin;
            }
        }
    }


    {
        vector<ExplosionObject*> toRemove;
        {
            vector<MapSubsetStruct<ExplosionObject*>*>::iterator
            begin = explosions->getObjectVector()->begin(),
                    end = explosions->getObjectVector()->end();
            for (;begin != end; begin++) {
                if ((*begin)->object->isFaded(currentTime)) {
                    toRemove.push_back((*begin)->object);
                }
            }
        }

        {
            vector<ExplosionObject*>::iterator
            begin = toRemove.begin(),
                    end = toRemove.end();
            for (;begin != end; begin++) {
                delete explosions->removeAt((*begin)->getLocationX(), (*begin)->getLocationZ());
            }
        }
    }



    if (died(player1)) {
        player1->die();
        printf("Some player died!\n");
    }
    if (died(player2)) {
        player2->die();
        printf("Some player died!\n");
    }
}


int main(int argc, char *argv[]) {

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
    bombModel = new ModelLoader("lolly.3DS");
    explosionModel = new ModelLoader("lolly.3DS");

    /* MAIN LOOP */
    clock_t lastTime = clock(), currTime = clock();
    while (keepRunning) {
        doEvents();	//Process incoming events.

        currTime = clock();
        float updateTime = ((float)(currTime - lastTime)) / CLOCKS_PER_SEC;
        lastTime = currTime;
        while (updateTime > 100) {
            doGameUpdate(100, ((float)currTime) / CLOCKS_PER_SEC);	//Update
            updateTime -= 100;
        }
        doGameUpdate(updateTime, ((float)currTime) / CLOCKS_PER_SEC);	//Update

        doDisplay();	//Draw the screen.
    }

    SDL_Quit();

    return 0;
}

