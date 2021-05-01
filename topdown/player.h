#ifndef PLAYER_H
#define PLAYER_H
#include "sdlinclude.h"
#include <stdbool.h>

typedef struct Player_type* Player;


Player createPlayer(int x, int y, int id);
void movePlayer(Player p, int up, int down, int right, int left, int mouseX, int mouseY);
SDL_Rect* getPlayerRect(Player p);
int getPlayerFrame(Player p);
double getPlayerDirection(Player p);
int getPlayerX(Player p);
int getPlayerY(Player p);
int getPlayerID(Player p);
void activatePlayer(Player p);
void playerHealth(Player p, int health);
void updatePlayerPosition(Player p, int x, int y, int rotation);
void moveOtherPlayers(Player p);
void snapPlayer(Player p, int x, int y);
void damagePlayer(Player p, int damage);
bool isPlayerAlive(Player p);
void setPlayerAlive(Player p, bool value);

#endif
