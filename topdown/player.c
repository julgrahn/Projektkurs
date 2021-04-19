#include <stdlib.h>
#include "player.h"
#include <SDL.h>
//#include <SDL2/SDL.h>

#define WINDOWWIDTH 704
#define WINDOWHEIGHT 704

#define PUBLIC
#define SPEED 5
#define HEALTH 100

//Player 
struct Player_type {
    int health;
    int speed;
    int isMoving;
    SDL_Rect pDimensions;
};

PUBLIC Player createPlayer(int x, int y)
{
    //Declare player health
    Player a = malloc(sizeof(struct Player_type));

    //Player health
    a->health = HEALTH;
    a->speed = SPEED;
    a->isMoving = 0;

    //Player size and starting position
    a->pDimensions.x = (WINDOWWIDTH - 64) / 2;
    a->pDimensions.y = (WINDOWHEIGHT - 64) / 2;
    a->pDimensions.w = 64;
    a->pDimensions.h = 64;

    return a;
}

PUBLIC void movePlayer(Player p, int up, int down, int right, int left)
{
    //Player movement
    p->isMoving = 0;
    if (up    && !down)    p->pDimensions.y -= SPEED; p->isMoving = 1;
    if (down  && !up)      p->pDimensions.y += SPEED; p->isMoving = 1;
    if (left  && !right)   p->pDimensions.x -= SPEED; p->isMoving = 1;
    if (right && !left)    p->pDimensions.x += SPEED; p->isMoving = 1;
    
    // Collision detection with window
    if (p-> pDimensions.y <= 0 ) p->pDimensions.y = 0;
    if (p-> pDimensions.y >= WINDOWHEIGHT-p->pDimensions.w ) p->pDimensions.y = WINDOWHEIGHT-p->pDimensions.w;
    if (p-> pDimensions.x <= 0 ) p->pDimensions.x = 0;
    if (p-> pDimensions.x >= WINDOWWIDTH-p->pDimensions.h ) p->pDimensions.x = WINDOWWIDTH-p->pDimensions.h;

}

//Function declaration for player health

PUBLIC void playerHealth(Player p, int health)
{

}

PUBLIC SDL_Rect getPlayerRect(Player p)
{
    return p->pDimensions;
}