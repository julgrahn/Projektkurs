#ifndef PLAYER_H
#define PLAYER_H
#include "sdlinclude.h"
#include <stdbool.h>

typedef struct Player_type* Player;


Player createPlayer(int x, int y);
void movePlayer(Player p, int up, int down, int right, int left, int mouseX, int mouseY, bool reload);
SDL_Rect* getPlayerRect(Player p);
int getPlayerFrame(Player p);
double getPlayerDirection(Player p);
int getPlayerX(Player p);
int getPlayerY(Player p);
int getPlayerID(Player p);
void activatePlayer(Player p);
int getPlayerHealth(Player p);
void updatePlayerPosition(Player p, int x, int y, int direction, bool alive);
void moveOtherPlayers(Player p);
void snapPlayer(Player p, int x, int y);
void damagePlayer(Player p, int damage);
bool isPlayerAlive(Player p);
void setPlayerAlive(Player p, bool value);
void clientDamagePlayer(Player p);
bool checkIfPlayerdamaged(Player p);
void resetDamagedPlayer(Player p);
void clientDamagePlayer(Player p);

bool canShoot(Player a);
void playerTick(Player a);
int getPlayerGunbarrelX(Player a);
int getPlayerGunbarrelY(Player a);
int getPlayerWeapondamage(Player a);
int getPlayerweaponMag(Player a);
void setPlayerhealth(Player a, int health);
void setPlayerLives(Player a, int lives);
int getPlayerlives(Player a);
int getPlayerReloadprogress(Player a);
void resetPlayer(Player a);
double getPlayerShotAngle(Player a);
bool checkKilled(Player a);

#endif
