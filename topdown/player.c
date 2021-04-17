#include <stdlib.h>
#include "player.h"
#include <SDL.h>

#define PUBLIC
#define SPEED 10

struct Player_type {
    int health;
    int speed;
    SDL_Rect pDimensions;
};

PUBLIC Player createPlayer(int x, int y)
{
    Player a = malloc(sizeof(struct Player_type));
    a->health = 100;
    a->speed = SPEED;

    a->pDimensions.x = x;
    a->pDimensions.y = y;
    a->pDimensions.w = 64;
    a->pDimensions.h = 64;

    return a;
}

PUBLIC void movePlayer(Player p, int up, int down, int right, int left)
{
    if (up && !down) p->pDimensions.y -= SPEED;
    if (down && !up) p->pDimensions.y += SPEED;
    if (left && !right) p->pDimensions.x -= SPEED;
    if (right && !left) p->pDimensions.x += SPEED;
}

PUBLIC SDL_Rect getPlayerRect(Player p)
{
    return p->pDimensions;
}