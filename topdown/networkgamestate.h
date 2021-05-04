#ifndef NETWORKGAMESTATE_H
#define NETWORKGAMESTATE_H

#include "sdlinclude.h"
#include "player.h"
// #include "networkbullet.h"
// #include "networkplayer.h"

typedef struct Networkgamestate_type* Networkgamestate;
// typedef struct Networkgamestate_type {
//     Networkplayer players[5];
// }Networkgamestate;

Networkgamestate createNetworkgamestate();
void* getNetworkgamestateplayer(Networkgamestate* a, int n);
void setGamastateplayerpos(Networkgamestate *a, int n, int x, int y);

void killNetworkgamestateplayer(Networkgamestate *a, int n);
void reviveNetworkgamestateplayer(Networkgamestate *a, int n);
bool isNetworkplayeractive(Networkgamestate *a, int n);
void freeNetworkgamestateplayer(Networkgamestate *a, int n);
void activateNetworkgamestateplayer(Networkgamestate *a, int n);
void setGamestateplayer(Networkgamestate *a, void *player, int n);
Uint32 getNetworkplayersize();
void setNetworkgamestateplayer(Networkgamestate *a, int n, Player b);
int getNetworkgamestateplayerX(Networkgamestate *a, int n);
int getNetworkgamestateplayerY(Networkgamestate *a, int n);
int getNetworkgamestateplayerDirection(Networkgamestate *a, int n);
bool isNetworkgamestateplayerAlive(Networkgamestate *a, int n);
int getNetworkgamestateplayerXtarget(Networkgamestate *a, int n);
int getNetworkgamestateplayerYtarget(Networkgamestate *a, int n);
bool isNetworkgamestateplayerShooting(Networkgamestate *a, int n);
Uint32 getGamestatesize();
void setNetworkplayeralive(Networkgamestate *a, int n, bool alive);
bool iisplayershoot(void *player);

// void setGamestateplayerID(Networkplayer *a, int id);
// int getGamestateplayerID(Networkplayer *a, int n);

#endif