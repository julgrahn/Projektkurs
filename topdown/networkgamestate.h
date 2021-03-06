#ifndef NETWORKGAMESTATE_H
#define NETWORKGAMESTATE_H

#include "sdlinclude.h"
#include "player.h"
#include "bullet.h"
#include <stdint.h>

typedef struct Networkgamestate_type* Networkgamestate;

Networkgamestate createNetworkgamestate();
Uint32 getGamestatesize();

void setRoundState(Networkgamestate a, int newValue);
Uint8 getRoundState(Networkgamestate a);

void* getNetPlayer(Networkgamestate a, int n);
void killNetPlayer(Networkgamestate a, int n);
void reviveNetPlayer(Networkgamestate a, int n);
bool isNetPlayerActive(Networkgamestate a, int n);
void freeNetPlayer(Networkgamestate a, int n);
void activateNetPlayer(Networkgamestate a, int n);
Uint32 getNetPlayerSize();
void setNetPlayer(Networkgamestate a, int n, Player b); // Copies important values from a regular player struct to a networkplayer struct
void setNetplayerLives(Networkgamestate a, int playerID, int lives);
int getNetPlayerX(Networkgamestate a, int n);
int getNetPlayerY(Networkgamestate a, int n);
int getNetPlayerDirection(Networkgamestate a, int n);
bool isNetPlayerAlive(Networkgamestate a, int n);
void setNetPlayerAlive(Networkgamestate a, int n, bool alive);
void damageNetplayer(Networkgamestate a, int playerID, int damage, int shooterID);
int getNetPlayerKills(Networkgamestate a, int n);
void setNetPlayerKills(Networkgamestate a, int n, int newValue);
int getNetplayerHealth(Networkgamestate a, int playerID);
bool isNetplayerInvulnerable(Networkgamestate a, int playerID);
void setNetplayerInvulnerable(Networkgamestate a, int playerID, bool value);
int getNetplayerLives(Networkgamestate a, int playerID);
void setNetplayerHealth(Networkgamestate a, int playerID, int health);
void setNetplayerPos(Networkgamestate a, int playerID, int x, int y);

void setNetBullets(Networkgamestate a, int playerID, Bullet bullets[]); // Copies important values from a regular bullet struct to a networkbullet struct
int getNetBulletX(Networkgamestate a, int playerID, int bulletNo);
int getNetBulletY(Networkgamestate a, int playerID, int bulletNo);
bool isNetbulletActive(Networkgamestate a, int playerID, int bulletNo);
void freeNetbullet(Networkgamestate a, int playerID, int bulletNo);
bool netbulletStatus(Networkgamestate a, int playerID, int bulletID);
void netBulletclearcontrol(Networkgamestate a, int playerID, int bulletID);
double getNetbulletspeedX(Networkgamestate a, int playerID, int bulletID);
double getNetbulletspeedY(Networkgamestate a, int playerID, int bulletID);
double getNetbulletAngle(Networkgamestate a, int playerID, int bulletID);
int getNetbulletdamage(Networkgamestate a, int playerID, int bulletID);

uint32_t* getWallState(Networkgamestate a, int playerID);
void combineWallstates(Networkgamestate a, uint32_t wallstates[]); // Merge the wall states of the clients with the server
void resetWallStates(Networkgamestate a, uint32_t wallstates[]);
void createWalls(Networkgamestate a, bool b);


#endif