#include <stdlib.h>
#include "player.h"
#include "world.h"
#include "weapon.h"
#include <math.h>

#define PUBLIC
#define SPEED 2
#define ANIMATIONSPEED 8               //lower = faster
#define HEALTH 100
#define ROTATION_UPDATE_SPEED 5
#define SNAP_DISTANCE 10

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
    bool alive;
    int id;
    int newX;
    int newY;
    double xSpeed, ySpeed;
    int newDirection;
    int xTarget, yTarget;
    bool isShooting;
    Weapon gun;
    bool wasDamaged;
    int gunBarrelX, gunBarrelY;
    int lives;
};

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
    a->alive = false;
    a->id = id;
    a->xSpeed = a->ySpeed = 0;
    a->newDirection = 0;
    a->isShooting = false;
    a->gun = createWeapon();
    a->wasDamaged = false;
    return a;
}

PUBLIC int getPlayerFrame(Player p)
{
    return p->frame;
}

PUBLIC void movePlayer(Player p, int up, int down, int right, int left, int mouseX, int mouseY, bool reload)
{
    if(reload) reloadWeapon(p->gun);
    int newX = 0, newY = 0, diagonal, oldX = p->posX, oldY = p->posY;
    p->isMoving = 0;
    p->xTarget = mouseX;
    p->yTarget = mouseY;
    if (up && !down) { newY--; p->isMoving = 1; }
    if (down && !up) { newY++; p->isMoving = 1; }
    if (left && !right) { newX--; p->isMoving = 1; }
    if (right && !left) { newX++; p->isMoving = 1; }
    diagonal = (newX != 0 && newY != 0);
    // Set player absolute pos
    p->posX += p->diaSpeed * diagonal * newX + p->speed * !diagonal * newX;
    p->posY += p->diaSpeed * diagonal * newY + p->speed * !diagonal * newY;

    // Set new pixel pos of player
    p->pDimensions.x = round(p->posX);
    if (getWallCollisionPlayer(p->pDimensions.x, p->pDimensions.y))   // Collision x-led
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
    if (getWallCollisionPlayer(p->pDimensions.x, p->pDimensions.y))   // Collision y-led
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

PUBLIC int getPlayerHealth(Player p)
{
    return p->health;
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

PUBLIC void activatePlayer(Player p)
{
    p->active = true;
}

PUBLIC int getPlayerID(Player p)
{
    return p->id;
}

PUBLIC void updatePlayerPosition(Player *p, int x, int y, int direction, bool alive) //bool isShooting, int xTarget, int yTarget)
{
    (*p)->alive = alive;
    (*p)->newX = x;
    (*p)->newY = y;
    (*p)->newDirection = direction;
    (*p)->direction = direction;
}

PUBLIC void updateServerPlayer(Player* p, int x, int y, int direction, bool alive, bool isShooting, int xTarget, int yTarget)
{
    (*p)->pDimensions.x = x;
    (*p)->pDimensions.y = y;
    (*p)->newDirection = direction;
    (*p)->direction = direction;
    (*p)->isShooting = isShooting;
    (*p)->xTarget = xTarget, (*p)->yTarget = yTarget;
}

PUBLIC void moveOtherPlayers(Player p)
{
    int xDelta = p->newX - p->pDimensions.x;
    int yDelta = p->newY - p->pDimensions.y;
    double distance = sqrt(xDelta * xDelta + yDelta * yDelta);
    double scaling = p->speed / (distance * (distance >= 1) + (distance < 1));
    if (distance >= SNAP_DISTANCE)
    {
        snapPlayer(p, p->newX, p->newY);
        return;
    }
    int old = p->direction + 180 + 5;
    int new = p->newDirection + 180 + 5;

    if (xDelta > 1 || xDelta < -1 || yDelta > 1 || yDelta < -1)
    {
        p->xSpeed = scaling * xDelta;
        p->ySpeed = scaling * yDelta;

        p->posX += p->xSpeed;
        p->posY += p->ySpeed;

        p->pDimensions.x = round(p->posX);
        p->pDimensions.y = round(p->posY);
        p->frameCounter = (p->frameCounter + 1) % (ANIMATIONSPEED + 1);
        p->frame = (p->frame + ((p->frameCounter / ANIMATIONSPEED))) % 4;
    }
    // if(p->direction != p->newDirection)
    // {
    //     if(new - old < 180 && new - old > 0)
    //     {
    //         old += ROTATION_UPDATE_SPEED;
    //     }
    //     else if(new - old > -180 && new - old < 0)
    //     {
    //         old -= ROTATION_UPDATE_SPEED;
    //     }
    //     else if(new - old < -180)
    //     {
    //             old += ROTATION_UPDATE_SPEED;
    //         if(old > 360)
    //         {
    //             old -= 360;
    //         }
    //     }
    //     else if(new - old > 180)
    //     {
    //         old -= ROTATION_UPDATE_SPEED;
    //         if(old < 0)
    //         {
    //             old += 360;
    //         }
    //         // if(new - old < ROTATION_UPDATE_SPEED) old = new;
    //     }
    //     old -= (180+5);
    //     p->direction = old;
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

PUBLIC void damagePlayer(Player p, int damage)
{
    p->health -= damage;
    if (p->health <= 0) p->alive = false;
}

PUBLIC void clientDamagePlayer(Player p)
{
    p->wasDamaged = true;
}

PUBLIC bool checkIfPlayerdamaged(Player p)
{
    return p->wasDamaged;
}

PUBLIC void resetDamagedPlayer(Player p)
{
    p->wasDamaged = false;
}

PUBLIC bool isPlayerAlive(Player p)
{
    return p->alive;
}

PUBLIC void setPlayerAlive(Player p, bool value)
{
    p->health = HEALTH;
    p->alive = value;
}

PUBLIC int getPlayerxtarget(Player a)
{
    return a->xTarget;
}

PUBLIC int getPlayerytarget(Player a)
{
    return a->yTarget;
}

PUBLIC bool isPlayershooting(Player a)
{
    // if(a->alive)
    return a->isShooting;
    // else
    //     return false;
}

PUBLIC void setPlayerShooting(Player* a, bool isShooting, int xTarget, int yTarget)
{
    (*a)->isShooting = isShooting;
    (*a)->xTarget = xTarget, (*a)->yTarget = yTarget;
}

PUBLIC bool canShoot(Player a)
{
    return fireWeapon(a->gun);
}

PUBLIC void playerTick(Player a)
{
    weaponTick(a->gun);
}

PUBLIC int getPlayerGunbarrelX(Player a)
{
    return a->pDimensions.x + 20 + (34*sin((-a->direction + 72)*M_PI/180));   
}

PUBLIC int getPlayerGunbarrelY(Player a)
{
    return a->pDimensions.y + 32 + (34*cos((-a->direction + 72)*M_PI/180));
}

PUBLIC int getPlayerWeapondamage(Player a)
{
    return getWeapondamage(a->gun);
}

PUBLIC int getPlayerweaponMag(Player a)
{
    return getMag(a->gun);
}

PUBLIC void setPlayerhealth(Player a, int health)
{
    a->health = health;
}

PUBLIC void setPlayerLives(Player a, int lives)
{
    a->lives = lives;
}

PUBLIC int getPlayerlives(Player a)
{
    return a->lives;
}

PUBLIC int getPlayerReloadprogress(Player a)
{
    return getReloadprogress(a->gun);
}

PUBLIC void resetPlayer(Player a)
{
    resetWeapon(a->gun);
}