#include <SDL.h>
#include <SDL_mixer.h>
#include <freeglut.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include "unbreakableobject.h"
#include "breakableobject.h"
#include "bombobject.h"
#include "explosionobject.h"
#include "mapsubset.h"
#include "playerobject.h"
#include "maputilities.h"
#include "cube2.h"
#include "modelloader.h"
#include <map>
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
map<BreakableObject*, BreakableObject*>* brokenWalls;
MapSubset<BombObject*>* bombs;
MapSubset<ExplosionObject*>* explosions;
MapSubset<bool>* spawns;
PlayerObject* player1;
PlayerObject* player2;
Model* bombModel = NULL;
Model* explosionModel = NULL;
static const int BOMBRADIUS = 3;
static const int EXPLOSIONLIFETIME = 1;
static const int WINDELAY = 3;
float victoryTimer = 0;

Mix_Chunk *explosionSound1;
Mix_Chunk *explosionSound2;
Mix_Chunk *dieSound1;
Mix_Chunk *dieSound2;
Mix_Chunk *victorySound1;
Mix_Chunk *victorySound2;



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
    MapUtilities::readMap("maze.data", unbreakableWalls, breakableWalls, new ModelLoader("cube_frame_subsurf_2.3ds"), new ModelLoader("cube_frame_subsurf_2.3ds"),0.5, '#', '%');

    // Place a player!
    if (player1 != NULL) {
        delete player1;
    }
	Model* playerModel = new ModelLoader("lolly.3DS");
    player1 = new PlayerObject(playerModel, 1, 1, 0, 180.0, 0.01, GL_LIGHT2);
    player2 = new PlayerObject(playerModel, 8, 8, 0, 180.0, 0.01, GL_LIGHT1);
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


void explosionAt(int x, int z, float endTime, Mix_Chunk *explosionSound);


bool tryPlaceExplosion(int x, int z, float endTime, Mix_Chunk *explosionSound) {
    if (isValidPosition(x, z)) {
        if (isBombAt(x, z)) {
            bombs->removeAt(x, z);
            explosionAt(x, z, endTime, explosionSound);
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


void explosionAt(int x, int z, float endTime, Mix_Chunk *explosionSound) {
	Mix_PlayChannel(-1, explosionSound, 0);
	tryPlaceExplosion(x, z, endTime, explosionSound);

	if (tryPlaceExplosion(x, z-1, endTime, explosionSound)) {
		if (tryPlaceExplosion(x, z-2, endTime, explosionSound)) {
			if (!tryPlaceExplosion(x, z-3, endTime, explosionSound)) {
				if (breakableWalls->isObjectAt(x, z-3)) {
					brokenWalls->insert(pair<BreakableObject*, BreakableObject*>(breakableWalls->getObjectAt(x, z-3), breakableWalls->getObjectAt(x, z-3)));
				}
			}
		} else {
			if (breakableWalls->isObjectAt(x, z-2)) {
				brokenWalls->insert(pair<BreakableObject*, BreakableObject*>(breakableWalls->getObjectAt(x, z-2), breakableWalls->getObjectAt(x, z-2)));
			}
		}
	} else {
		if (breakableWalls->isObjectAt(x, z-1)) {
			brokenWalls->insert(pair<BreakableObject*, BreakableObject*>(breakableWalls->getObjectAt(x, z-1), breakableWalls->getObjectAt(x, z-1)));
		}
	}

	if (tryPlaceExplosion(x, z+1, endTime, explosionSound)) {
		if (tryPlaceExplosion(x, z+2, endTime, explosionSound)) {
			if (!tryPlaceExplosion(x, z+3, endTime, explosionSound)) {
				if (breakableWalls->isObjectAt(x, z+3)) {
					brokenWalls->insert(pair<BreakableObject*, BreakableObject*>(breakableWalls->getObjectAt(x, z+3), breakableWalls->getObjectAt(x, z+3)));
				}
			}
		} else {
			if (breakableWalls->isObjectAt(x, z+2)) {
				brokenWalls->insert(pair<BreakableObject*, BreakableObject*>(breakableWalls->getObjectAt(x, z+2), breakableWalls->getObjectAt(x, z+2)));
			}
		}
	} else {
		if (breakableWalls->isObjectAt(x, z+1)) {
			brokenWalls->insert(pair<BreakableObject*, BreakableObject*>(breakableWalls->getObjectAt(x, z+1), breakableWalls->getObjectAt(x, z+1)));
		}
	}

	if (tryPlaceExplosion(x-1, z, endTime, explosionSound)) {
		if (tryPlaceExplosion(x-2, z, endTime, explosionSound)) {
			if (!tryPlaceExplosion(x-3, z, endTime, explosionSound)) {
				if (breakableWalls->isObjectAt(x-3, z)) {
					brokenWalls->insert(pair<BreakableObject*, BreakableObject*>(breakableWalls->getObjectAt(x-3, z), breakableWalls->getObjectAt(x-3, z)));
				}
			}
		} else {
			if (breakableWalls->isObjectAt(x-2, z)) {
				brokenWalls->insert(pair<BreakableObject*, BreakableObject*>(breakableWalls->getObjectAt(x-2, z), breakableWalls->getObjectAt(x-2, z)));
			}
		}
	} else {
		if (breakableWalls->isObjectAt(x-1, z)) {
			brokenWalls->insert(pair<BreakableObject*, BreakableObject*>(breakableWalls->getObjectAt(x-1, z), breakableWalls->getObjectAt(x-1, z)));
		}
	}

	if (tryPlaceExplosion(x+1, z, endTime, explosionSound)) {
		if (tryPlaceExplosion(x+2, z, endTime, explosionSound)) {
			if (!tryPlaceExplosion(x+3, z, endTime, explosionSound)) {
				if (breakableWalls->isObjectAt(x+3, z)) {
					brokenWalls->insert(pair<BreakableObject*, BreakableObject*>(breakableWalls->getObjectAt(x+3, z), breakableWalls->getObjectAt(x+3, z)));
				}
			}
		} else {
			if (breakableWalls->isObjectAt(x+2, z)) {
				brokenWalls->insert(pair<BreakableObject*, BreakableObject*>(breakableWalls->getObjectAt(x+2, z), breakableWalls->getObjectAt(x+2, z)));
			}
		}
	} else {
		if (breakableWalls->isObjectAt(x+1, z)) {
			brokenWalls->insert(pair<BreakableObject*, BreakableObject*>(breakableWalls->getObjectAt(x+1, z), breakableWalls->getObjectAt(x+1, z)));
		}
	}
}


void doGameUpdateForPlayer(float updateTime, float currentTime, PlayerObject* player, int left, int right, int up, int down, int bomb, Mix_Chunk *explosionSound) {
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
				bombs->insertAt(new BombObject(explosionSound, currentTime, bombModel, round(player->getLocationX()), round(player->getLocationZ()), 0, 0, 0.01), round(player->getLocationX()), round(player->getLocationZ()));
				printf("Dropped a bomb!\n");
				player->droppedBomb(currentTime);
			}
		}
	}
}


void doGameUpdate(float updateTime, float currentTime) {
	doGameUpdateForPlayer(updateTime, currentTime, player1, SDLK_LEFT, SDLK_RIGHT, SDLK_UP, SDLK_DOWN, SDLK_RSHIFT, explosionSound1);
	doGameUpdateForPlayer(updateTime, currentTime, player2, SDLK_a, SDLK_d, SDLK_w, SDLK_s, SDLK_LSHIFT, explosionSound2);

	
	{
        vector<BombObject*> toRemove;
        {
            vector<MapSubsetStruct<BombObject*>*>::iterator
            begin = bombs->getObjectVector()->begin(),
                    end = bombs->getObjectVector()->end();
            for (;begin != end; begin++) {
                if ((*begin)->object->isExploded(currentTime)) {
                    toRemove.push_back((*begin)->object);
				} else {
					(*begin)->object->updateScale(currentTime);
				}
            }
        }

        {
            vector<BombObject*>::iterator
            begin = toRemove.begin(),
                    end = toRemove.end();
            for (;begin != end; begin++) {
                BombObject* bomb = bombs->removeAt((*begin)->getLocationX(), (*begin)->getLocationZ());
				explosionAt((*begin)->getLocationX(), (*begin)->getLocationZ(), currentTime + EXPLOSIONLIFETIME, bomb->getSound());
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


    {
        map<BreakableObject*, BreakableObject*>::iterator
			begin = brokenWalls->begin(),
			end = brokenWalls->end();
        for (;begin != end; begin++) {
			breakableWalls->removeAt((*begin).first->getLocationX(), (*begin).first->getLocationZ());
			//delete;
        }
		brokenWalls->clear();
	}





	if (died(player1) && !player1->isDead()) {
		if (victoryTimer == 0) 
			victoryTimer = currentTime;

        player1->die();
		Mix_PlayChannel(-1, dieSound1, 0);
        printf("Some player died!\n");
    }
    if (died(player2) && !player2->isDead()) {
		if (victoryTimer == 0) 
			victoryTimer = currentTime;

		Mix_PlayChannel(-1, dieSound2, 0);
        player2->die();
        printf("Some player died!\n");
    }


	if (currentTime - victoryTimer > WINDELAY && victoryTimer != 0) {
		if (player1->isDead() && !player2->isDead()) {
			Mix_PlayChannel(-1, victorySound2, 0);
		} else if (!player1->isDead() && player2->isDead()) {
			Mix_PlayChannel(-1, victorySound1, 0);
		}
		player1->die();
		player2->die();
	}
}


int main(int argc, char *argv[]) {
    // Init the subsets and set global map width / height 
    MapUtilities::getMapSize("maze.data", levelHeight, levelWidth);
    unbreakableWalls=new MapSubset<UnbreakableObject*>(levelWidth, levelHeight);
    breakableWalls = new MapSubset<BreakableObject*>(levelWidth, levelHeight);
    brokenWalls = new map<BreakableObject*, BreakableObject*>();
    bombs = new MapSubset<BombObject*>(levelWidth, levelHeight);
    explosions = new MapSubset<ExplosionObject*>(levelWidth, levelHeight);
    spawns = new MapSubset<bool>(levelWidth, levelHeight);

    // SDL INITIATION
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

	// SOUND INITIATION
	int audio_rate = 44100;
	Uint16 audio_format = AUDIO_S16SYS;
	int audio_channels = 2;
	int audio_buffers = 4096;
	if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
		fprintf(stderr, "Unable to initialize audio: %s\n", Mix_GetError());
		exit(1);
	}
	explosionSound1 = Mix_LoadWAV("sounds/explosion1.wav");
	if(explosionSound1 == NULL) {
		fprintf(stderr, "Unable to load WAV file: %s\n", Mix_GetError());
	}
	explosionSound2 = Mix_LoadWAV("sounds/explosion2.wav");
	if(explosionSound2 == NULL) {
		fprintf(stderr, "Unable to load WAV file: %s\n", Mix_GetError());
	}
	dieSound1 = Mix_LoadWAV("sounds/die1.wav");
	if(dieSound1 == NULL) {
		fprintf(stderr, "Unable to load WAV file: %s\n", Mix_GetError());
	}
	dieSound2 = Mix_LoadWAV("sounds/die2.wav");
	if(dieSound2 == NULL) {
		fprintf(stderr, "Unable to load WAV file: %s\n", Mix_GetError());
	}
	victorySound1 = Mix_LoadWAV("sounds/victory1.wav");
	if(victorySound1 == NULL) {
		fprintf(stderr, "Unable to load WAV file: %s\n", Mix_GetError());
	}
	victorySound2 = Mix_LoadWAV("sounds/victory2.wav");
	if(victorySound2 == NULL) {
		fprintf(stderr, "Unable to load WAV file: %s\n", Mix_GetError());
	}



    // OPENGL SETUP 
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_SetVideoMode(size[0], size[1], 0, SDL_OPENGL | SDL_HWSURFACE | SDL_RESIZABLE);

    glClearColor((GLfloat)0.8, (GLfloat)1.0, (GLfloat)1.0, (GLfloat)1);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);

	{
		GLfloat vector[4];
		vector[0] = 1;
		vector[1] = 1;
		vector[2] = 1;
		vector[3] = 1;
		glLightfv(GL_LIGHT0, GL_DIFFUSE, vector);
		glLightfv(GL_LIGHT0, GL_SPECULAR, vector);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, vector);
		glLightfv(GL_LIGHT1, GL_SPECULAR, vector);
		glLightfv(GL_LIGHT2, GL_DIFFUSE, vector);
		glLightfv(GL_LIGHT2, GL_SPECULAR, vector);
		vector[0] = 4.5;
		vector[1] = 0;
		vector[2] = 4.5;
		vector[3] = 1;
		glLightfv(GL_LIGHT0, GL_POSITION, vector);
		glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 1);
		glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.1);
		glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1);
		glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 1);
		glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.1);
		glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1);
		glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 1);
		glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.1);
		glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1);
	}

	glEnable(GL_TEXTURE_2D);
    glEnable(GL_NORMALIZE);

    reshape(size[0], size[1]);

    // GAME SETUP 
    loadLevel();
    bombModel = new ModelLoader("lolly.3DS");
    explosionModel = new ModelLoader("lolly.3DS");

    // MAIN LOOP 
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

