#include <stdlib.h>
#include "player.h"
<<<<<<< HEAD
//#include <SDL.h>
#include <SDL2/SDL.h>
=======
#include <SDL.h>
//#include <SDL2/SDL.h>
>>>>>>> e00810607ca7030f068fa815f3bb1e47fe9a5eb2

#define WINDOWWIDTH 704
#define WINDOWHEIGTH 704

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
<<<<<<< HEAD
    
    // Collision detection with window
    if (p->pDimensions.y <= 0 ) p->pDimensions.y = 0;
    if (p->pDimensions.y >= WINDOWHEIGTH-p->pDimensions.w ) p->pDimensions.y = WINDOWHEIGTH-p->pDimensions.w;
    if (p->pDimensions.x <=0 ) p->pDimensions.x = 0;
    if (p->pDimensions.x >= WINDOWWIDTH-p->pDimensions.h ) p->pDimensions.x = WINDOWWIDTH-p->pDimensions.h;
=======

    // Collision detection with window
    if (p->pDimensions.y <= 0) p->pDimensions.y = 0;
    if (p->pDimensions.y >= WINDOWHEIGTH - p->pDimensions.w) p->pDimensions.y = WINDOWHEIGTH - p->pDimensions.w;
    if (p->pDimensions.x <= 0) p->pDimensions.x = 0;
    if (p->pDimensions.x >= WINDOWWIDTH - p->pDimensions.h) p->pDimensions.x = WINDOWWIDTH - p->pDimensions.h;
>>>>>>> e00810607ca7030f068fa815f3bb1e47fe9a5eb2

}

PUBLIC SDL_Rect getPlayerRect(Player p)
{
    return p->pDimensions;
}