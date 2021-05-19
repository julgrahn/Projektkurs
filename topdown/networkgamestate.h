#ifndef NETWORKGAMESTATE_H
#define NETWORKGAMESTATE_H

#include "sdlinclude.h"
#include "player.h"
#include "bullet.h"

typedef struct Networkgamestate_type* Networkgamestate;

Networkgamestate createNetworkgamestate();
void* getNetPlayer(Networkgamestate a, int n);
void killNetPlayer(Networkgamestate a, int n);
void reviveNetPlayer(Networkgamestate a, int n);
bool isNetPlayerActive(Networkgamestate a, int n);
void freeNetPlayer(Networkgamestate a, int n);
void activateNetPlayer(Networkgamestate a, int n);
Uint32 getNetPlayerSize();
void setNetPlayer(Networkgamestate a, int n, Player b);
void setNetplayerLives(Networkgamestate a, int playerID, int lives);
int getNetPlayerX(Networkgamestate a, int n);
int getNetPlayerY(Networkgamestate a, int n);
int getNetPlayerDirection(Networkgamestate a, int n);
bool isNetPlayerAlive(Networkgamestate a, int n);
Uint32 getGamestatesize();
void setNetPlayerAlive(Networkgamestate a, int n, bool alive);

void setNetBullets(Networkgamestate a, int playerID, Bullet bullets[]);
int getNetBulletX(Networkgamestate a, int playerID, int bulletNo);
int getNetBulletY(Networkgamestate a, int playerID, int bulletNo);
bool isNetbulletActive(Networkgamestate a, int playerID, int bulletNo);
void damageNetplayer(Networkgamestate a, int playerID, int damage);
int getNetplayerHealth(Networkgamestate a, int playerID);
bool isNetplayerInvulnerable(Networkgamestate a, int playerID);
void setNetplayerInvulnerable(Networkgamestate a, int playerID, bool value);
int getNetplayerLives(Networkgamestate a, int playerID);
void setNetplayerHealth(Networkgamestate a, int playerID, int health);
void freeNetbullet(Networkgamestate a, int playerID, int bulletNo);
void setNetplayerPos(Networkgamestate a, int playerID, int x, int y);
bool netbulletStatus(Networkgamestate a, int playerID, int bulletID);
void netBulletclearcontrol(Networkgamestate a, int playerID, int bulletID);
double getNetbulletspeedX(Networkgamestate a, int playerID, int bulletID);
double getNetbulletspeedY(Networkgamestate a, int playerID, int bulletID);
double getNetbulletAngle(Networkgamestate a, int playerID, int bulletID);
int getNetbulletdamage(Networkgamestate a, int playerID, int bulletID);
bool getNetPlayerKilled(Networkgamestate a, int playerID);
void resetPlayerKilled(Networkgamestate a, int playerID);

#endif