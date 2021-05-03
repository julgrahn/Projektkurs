#ifndef NETWORKGAMESTATE_H
#define NETWORKGAMESTATE_H

#include "sdlinclude.h"
#include "networkbullet.h"
#include "networkplayer.h"

// typedef struct Networkgamestate_type* Networkgamestate;
typedef struct Networkgamestate_type {
    Networkplayer players[5];
}Networkgamestate;

Networkgamestate createNetworkgamestate();
Networkplayer* getNetworkgamestateplayer(Networkgamestate* a, int playerID);

#endif