#include <stdlib.h>
#include "player.h"
#include <math.h>
#include <stdbool.h>

#define PUBLIC
#define SPEED 2
#define ANIMATIONSPEED 8               //lower = faster
#define HEALTH 100

struct Player_type {
    int health;
    double speed;
    double diaSpeed;
    double posX, posY;
    SDL_Rect pDimensions;
    int frame;
    int frameCounter;
    int isMoving;
    double direction;
    bool active;
    int id;
    int newX;
    int newY;
    double xSpeed, ySpeed;
};

//int SDL_RenderDrawRect(SDL_Renderer* renderer, SDL_Rect NULL);

PUBLIC Player createPlayer(int x, int y, int id)
{
    Player a = malloc(sizeof(struct Player_type));
    a->health = HEALTH;
    a->speed = SPEED;
    a->diaSpeed = sqrt(SPEED * SPEED / 2);
    a->frame = 0;
    a->isMoving = 0;
    a->frameCounter = 0;
    a->direction = 0;
    a->posX = x;
    a->posY = y;
    a->pDimensions.x = x;
    a->pDimensions.y = y;
    a->pDimensions.w = 64;
    a->pDimensions.h = 64;
    a->newX = x;
    a->newY = y;
    a->active = false;
    a->id = id;
    a->xSpeed = a->ySpeed = 0;
    return a;
}

PUBLIC int getPlayerFrame(Player p)
{
    return p->frame;
}

PUBLIC void movePlayer(Player p, int up, int down, int right, int left, int mouseX, int mouseY)
{
    int newX = 0, newY = 0, diagonal;
    p->isMoving=0;

    if (up && !down) {newY--;p->isMoving=1;}
    if (down && !up) {newY++;p->isMoving=1;}
    if (left && !right) {newX--;p->isMoving=1;}
    if (right && !left) {newX++;p->isMoving=1;}
    diagonal = (newX!=0 && newY!=0);
    // Set player absolute pos
    p->posX += p->diaSpeed*diagonal*newX + p->speed*!diagonal*newX;
    p->posY += p->diaSpeed*diagonal*newY + p->speed*!diagonal*newY;

    // Set new pixel pos of player
    p->pDimensions.x = round(p->posX);
    if (getWallCollision(p->pDimensions.x, p->pDimensions.y))   // Collision x-led
    {
        if (right)
        {
            p->posX -= 2;
            p->pDimensions.x -= 2;
        }
        if (left)
        {
            p->posX += 2;
            p->pDimensions.x += 2;
        }
    }
    p->pDimensions.y = round(p->posY);
    if (getWallCollision(p->pDimensions.x, p->pDimensions.y))   // Collision y-led
    {
        if (up)
        {
            p->posY += 2;
            p->pDimensions.y += 2;
        }
        if (down)
        {
            p->posY -= 2;
            p->pDimensions.y -= 2;
        }
    }

    // Update player sprite frame
    p->frameCounter = (p->frameCounter + p->isMoving) % (ANIMATIONSPEED + 1);
    p->frame = (p->frame + ((p->frameCounter / ANIMATIONSPEED) * p->isMoving)) % 4;
    // Rotate player
    p->direction = (atan2(mouseY - p->pDimensions.y - 34, mouseX - p->pDimensions.x - 18) * 180 / M_PI) - 6;

    // Collision detection with window
    if (p->pDimensions.y <= 0) p->pDimensions.y = p->posY = 0;
    if (p->pDimensions.y >= WINDOWHEIGHT - p->pDimensions.h) p->pDimensions.y = p->posY = WINDOWHEIGHT - p->pDimensions.h;
    if (p->pDimensions.x <= 0) p->pDimensions.x = p->posX = 0;
    if (p->pDimensions.x >= WINDOWWIDTH - p->pDimensions.w) p->pDimensions.x = p->posX = WINDOWWIDTH - p->pDimensions.w;
}

PUBLIC double getPlayerDirection(Player p)
{
    return p->direction;
}

PUBLIC void playerHealth(Player p, int health)
{

}

PUBLIC SDL_Rect* getPlayerRect(Player p)
{
    return &p->pDimensions;
}

PUBLIC int getPlayerX(Player p)
{
    return p->pDimensions.x;
}

PUBLIC int getPlayerY(Player p)
{
    return p->pDimensions.y;
}

/*PUBLIC void shoot(Player p, Bullet bullets[])
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (!isBulletActive(bullets[i]))
        {
            spawnBullet(p, bullets[i]);
            return;
        }
    }
}*/

PUBLIC void activatePlayer(Player p)
{
    p->active = true;
}

PUBLIC int getPlayerID(Player p)
{
    return p->id;
}

PUBLIC void updatePlayerPosition(Player p, int x, int y, int direction)
{

    p->newX = x;
    p->newY = y;
    p->direction = direction;
}

// PUBLIC void moveOtherPlayers(Player p)
// {
//     float x_vel;
//     float y_vel;
//     float delta_x = p->newX - p->pDimensions.x;
//     float delta_y = p->newY - p->pDimensions.y;
//     float distance = sqrt(delta_x * delta_x + delta_y * delta_y);
//     x_vel = delta_x * SPEED / distance;
//     y_vel = delta_y * SPEED / distance;


//     // p->posX += x_vel;
//     // p->posY += y_vel;

//     // p->pDimensions.x += round(p->posX);
//     // p->pDimensions.y += round(p->posY);

//     if (distance < 1)
//     {
//         x_vel = y_vel = 0;
//     }
//     else
//     {
//         p->pDimensions.x += x_vel;
//         p->pDimensions.y += y_vel;
//     }


//     // printf("%.2f %.2f %.2f %.2f\n", p->posX, p->posY, p->speed, p->diaSpeed);
//     // Set new pixel pos of player
   
    
// }

PUBLIC void moveOtherPlayers(Player p)
{
    int xDelta = p->newX - p->pDimensions.x;
    int yDelta = p->newY - p->pDimensions.y;
    double distance = sqrt(xDelta*xDelta + yDelta*yDelta);
    double scaling = p->speed/(distance*(distance >= 1)+(distance < 1));
    // if(xDelta > 1 || xDelta < -1 || yDelta > 1 || yDelta < -1)
    if(distance > 1)
    {
        // if(distance >= 0) scaling = p->speed/distance
        // scaling = p->speed/(distance*(distance >= 1)+(distance < 1));
        p->xSpeed = scaling*xDelta;
        p->ySpeed = scaling*yDelta;

        p->posX += p->xSpeed;
        p->posY += p->ySpeed;

        p->pDimensions.x = round(p->posX);
        p->pDimensions.y = round(p->posY);
        p->frameCounter = (p->frameCounter + 1) % (ANIMATIONSPEED + 1);
        p->frame = (p->frame + ((p->frameCounter / ANIMATIONSPEED))) % 4;
        // printf("%d %d\n", p->pDimensions.x, p->pDimensions.y);
    }
    // else
    // {
    //     p->pDimensions.x = p->posX = p->newX;
    //     p->pDimensions.y = p->posY = p->newY;
    // }
}

PUBLIC void snapPlayer(Player p, int x, int y)
{
    p->pDimensions.x = x;
    p->pDimensions.y = y;
    p->newX = x;
    p->newY = y;
    p->posX = x;
    p->posY = y;
}