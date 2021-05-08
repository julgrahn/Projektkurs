#ifndef NETWORKGAMESTATE_H
#define NETWORKGAMESTATE_H

#include "sdlinclude.h"
#include "player.h"
#include "bullet.h"

typedef struct Networkgamestate_type* Networkgamestate;

Networkgamestate createNetworkgamestate();
void* getNetworkgamestateplayer(Networkgamestate* a, int n);
void setGamastateplayerpos(Networkgamestate *a, int n, int x, int y);

void killNetworkplayer(Networkgamestate *a, int n);
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
bool isNetworkplayerAlive(Networkgamestate *a, int n);
Uint32 getGamestatesize();
void setNetworkplayeralive(Networkgamestate *a, int n, bool alive);

void setNetworkbullets(Networkgamestate a, int playerID, Bullet bullets[]);
int getNetbulletX(Networkgamestate a, int playerID, int bulletNo);
int getNetbulletY(Networkgamestate a, int playerID, int bulletNo);
bool isNetbulletActive(Networkgamestate a, int playerID, int bulletNo);
void damageNetplayer(Networkgamestate a, int playerID);
int getNetplayerHealth(Networkgamestate a, int playerID);
void setNetplayerHealth(Networkgamestate a, int playerID, int health);
void freeNetbullet(Networkgamestate a, int playerID, int bulletNo);
void setNetplayerPos(Networkgamestate a, int playerID, int x, int y);
bool netbulletStatus(Networkgamestate a, int playerID, int bulletID);
void netBulletclearcontrol(Networkgamestate a, int playerID, int bulletID);
double getNetbulletspeedX(Networkgamestate a, int playerID, int bulletID);
double getNetbulletspeedY(Networkgamestate a, int playerID, int bulletID);


// bool isNetworkgamestateplayerShooting(Networkgamestate *a, int n);
#endif