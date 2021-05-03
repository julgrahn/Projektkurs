#ifndef  NETWORKPLAYER_H
#define  NETWORKPLAYER_H

#include "sdlinclude.h"
#include "networkbullet.h"
#include "player.h"
#include "bullet.h"

// typedef struct Networkplayer_type *Networkplayer;
typedef struct Networkplayer_type {
    short id, health, direction;
    short posX, posY, xTarget, yTarget;
    bool isAlive, isShooting;
    // Networkbullet bullets[10];
}Networkplayer;

Networkplayer createNetworkplayer(int id);
// void copyPlayer(Networkplayer *dest, Networkplayer source);
// int getNetplayerX(Networkplayer a);
// unsigned long getNetplayersize();
// void setNetplayerX(Networkplayer *a, int x);
void setNetworkplayer(Networkplayer* a, Player b);
// Networkbullet getNetworkbullet(Networkplayer *a, int n);

#endif