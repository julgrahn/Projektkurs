#ifndef PLAYER_H
#define PLAYER_H
#include "sdlinclude.h"

typedef struct Player_type* Player;


Player createPlayer(int x, int y);
void movePlayer(Player p, int up, int down, int right, int left, SDL_Point mouseXY);
SDL_Rect* getPlayerRect(Player p);
int getPlayerFrame(Player p);
double getPlayerDirection(Player p);
int getPlayerX(Player p);
int getPlayerY(Player p);

void playerHealth(Player p, int health);

#endif
