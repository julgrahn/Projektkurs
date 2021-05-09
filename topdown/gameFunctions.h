#ifndef GAMEFUNCTIONS_H
#define GAMEFUNCTIONS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "bullet.h"
#include "player.h"
#include "sdlinclude.h"
#include "networkgamestate.h"

void updateplayers(Networkgamestate networkgamestate, Player players[], int playerID);
void playerBulletCollisionCheck(Bullet bullets[], Player players[]);
bool rectCollisionTest(SDL_Rect* a, SDL_Rect* b);
void fire(Bullet bullets[], Player* p, int playerID, int xTarget, int yTarget);


#endif  // GAMEFUNCTIONS_H