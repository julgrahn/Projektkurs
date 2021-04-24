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
    return a;
}

PUBLIC int getPlayerFrame(Player p)
{
    return p->frame;
}

PUBLIC void movePlayer(Player p, int up, int down, int right, int left, int mouseX, int mouseY)
{
    double newX = 0, newY = 0;
    p->isMoving = 0;
    if (up && !down) { newY -= p->speed; p->isMoving = 1; }
    if (down && !up) { newY += p->speed; p->isMoving = 1; }
    if (left && !right) { newX -= p->speed; p->isMoving = 1; }
    if (right && !left) { newX += p->speed; p->isMoving = 1; }
    // Calc player absolute pos accounting for diagonal movement scaling
    p->posX += (p->diaSpeed * (newX > 0) + p->diaSpeed * (newX < 0) * (-1)) * (newX != 0 && newY != 0) + newX * !(newX != 0 && newY != 0);
    p->posY += (p->diaSpeed * (newY > 0) + p->diaSpeed * (newY < 0) * (-1)) * (newX != 0 && newY != 0) + newY * !(newX != 0 && newY != 0);

    // printf("%.2f %.2f %.2f %.2f\n", p->posX, p->posY, p->speed, p->diaSpeed);
    // Set new pixel pos of player
    p->pDimensions.x = round(p->posX);
    p->pDimensions.y = round(p->posY);

    // Update player sprite frame
    p->frameCounter = (p->frameCounter + p->isMoving) % (ANIMATIONSPEED + 1);
    p->frame = (p->frame + ((p->frameCounter / ANIMATIONSPEED) * p->isMoving)) % 4;
    // Rotate player
    p->direction = (atan2(mouseY - p->pDimensions.y - 34, mouseX - p->pDimensions.x - 18) * 180 / M_PI) - 6;

    // Update player sprite frame
    p->frameCounter = (p->frameCounter+p->isMoving)%(ANIMATIONSPEED+1);
    p->frame = (p->frame+((p->frameCounter/ANIMATIONSPEED)*p->isMoving))%4;
    // Rotate player
    p->direction = atan2(mouseY-p->pDimensions.y-34, mouseX-p->pDimensions.x-18)*180/M_PI;
    
    // Collision detection with window
    if (p->pDimensions.y <= 0) p->pDimensions.y = p->posY = 0;
    if (p->pDimensions.y >= WINDOWHEIGHT - p->pDimensions.h) p->pDimensions.y = p->posY = WINDOWHEIGHT - p->pDimensions.h;
    if (p->pDimensions.x <= 0) p->pDimensions.x = p->posX = 0;
    if (p->pDimensions.x >= WINDOWWIDTH - p->pDimensions.w) p->pDimensions.x = p->posX = WINDOWWIDTH - p->pDimensions.w;

  

}

PUBLIC int getPlayerDirection(Player p)
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

PUBLIC void updatePlayerPosition(Player p, int x, int y)
{

    p->newX = x;
    p->newY = y;
}

PUBLIC void moveOtherPlayers(Player p)
{
    float x_vel;
    float y_vel;
    float delta_x = p->newX - p->pDimensions.x;
    float delta_y = p->newY - p->pDimensions.y;
    float distance = sqrt(delta_x * delta_x + delta_y * delta_y);
    x_vel = delta_x * SPEED / distance;
    y_vel = delta_y * SPEED / distance;

    if (distance < 1)
    {
        x_vel = y_vel = 0;
    }
    else
    {
        p->pDimensions.x += x_vel;
        p->pDimensions.y += y_vel;
    }


    // printf("%.2f %.2f %.2f %.2f\n", p->posX, p->posY, p->speed, p->diaSpeed);
    // Set new pixel pos of player
   
    
}