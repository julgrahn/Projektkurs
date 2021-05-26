#ifndef GAMEFUNCTIONS_H
#define GAMEFUNCTIONS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "bullet.h"
#include "button.h"
#include "server.h"
#include "player.h"
#include "sdlinclude.h"
#include "networkgamestate.h"
#include "weapon.h"

void updateplayers(Networkgamestate networkgamestate, Player players[], int playerID);
bool rectCollisionTest(SDL_Rect* a, SDL_Rect* b);
void fire(Bullet bullets[], Player p);
void updateplayerbullets(Networkgamestate networkgamestate, int playerID, Bullet *bullets);

#endif  // GAMEFUNCTIONS_H