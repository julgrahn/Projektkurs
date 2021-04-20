#ifndef PLAYER_H
#define PLAYER_H
#include <SDL.h>
//include <SDL2/SDL.h>
typedef struct Player_type* Player;


Player createPlayer(int x, int y);
void movePlayer(Player p, int up, int down, int right, int left, int mouseX, int mouseY);
SDL_Rect* getPlayerRect(Player p);
int getPlayerFrame(Player p);
int getPlayerDirection(Player p);

void playerHealth(Player p, int health);

#endif
