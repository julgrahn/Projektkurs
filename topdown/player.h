#ifndef PLAYER_H
#define PLAYER_H
#include "sdlinclude.h"

typedef struct Player_type* Player;

struct Player_type {
    int health;
    double speed;
    double diaSpeed;
    double posX, posY;
    int oldPosX, oldPosY;
    SDL_Rect pDimensions;
    int frame;
    int frameCounter;
    int isMoving;
    double direction;
};


Player createPlayer(int x, int y);
void movePlayer(Player p, int up, int down, int right, int left, int mouseX, int mouseY);
SDL_Rect* getPlayerRect(Player p);
int getPlayerFrame(Player p);
int getPlayerDirection(Player p);

void playerHealth(Player p, int health);

#endif
