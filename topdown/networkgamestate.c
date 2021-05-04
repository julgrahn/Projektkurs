#include "networkgamestate.h"

#define PUBLIC

typedef struct NetworkBullet_type{
	short xPos, yPos;
    Uint8 direction;
    bool active;
}Networkbullet;

typedef struct NetworkPlayer_type{
    short direction, health;
    short posX, posY, xTarget, yTarget;
    bool isAlive, isShooting, isActive;
    Uint8 damage;
    // Uint8 playerstatus, damage, health;
    // short bullets[15][4];
    // Networkbullet aBullet[15];
}Networkplayer;

struct Networkgamestate_type {
    Networkplayer aPlayer[5];
};

PUBLIC Networkgamestate createNetworkgamestate()
{
    Networkgamestate a = malloc(sizeof(struct Networkgamestate_type));
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        a->aPlayer[i].direction = 0;
        a->aPlayer[i].posX = 0, a->aPlayer[i].posY = 0;
        a->aPlayer[i].isAlive = false;
        a->aPlayer[i].isShooting = false;
        a->aPlayer[i].isActive = false;
        // for (int j = 0; j < 15; j++)
        // {
        //     a->aPlayer[i].aBullet[j].active = false;
        // }
        
        // a->aPlayer[i].xTarget = 0, a->aPlayer[i].yTarget = 0;
    }
    // sizeof(a->aPlayer->aBullet);
    return a;
}

PUBLIC Uint32 getGamestatesize()
{
    // sizeof(short);
    return sizeof(struct Networkgamestate_type);
}

PUBLIC int getNetworkgamestateplayerX(Networkgamestate *a, int n)
{
    return (*a)->aPlayer[n].posX;
}

PUBLIC int getNetworkgamestateplayerY(Networkgamestate *a, int n)
{
    return (*a)->aPlayer[n].posY;
}

PUBLIC void setGamastateplayerpos(Networkgamestate *a, int n, int x, int y)
{
    (*a)->aPlayer[n].posX = x, (*a)->aPlayer[n].posY = y;
}

PUBLIC void setNetworkgamestateplayer(Networkgamestate *a, int n, Player b)
{
    (*a)->aPlayer[n].isAlive = isPlayerAlive(b);
    (*a)->aPlayer[n].posX = getPlayerX(b);
    (*a)->aPlayer[n].posY = getPlayerY(b);
    (*a)->aPlayer[n].direction = (short)getPlayerDirection(b);
    (*a)->aPlayer[n].health = getPlayerHealth(b);
    (*a)->aPlayer[n].xTarget = getPlayerxtarget(b);
    (*a)->aPlayer[n].yTarget = getPlayerytarget(b);
    if(isPlayerAlive(b)) (*a)->aPlayer[n].isShooting = isPlayershooting(b);
    else (*a)->aPlayer[n].isShooting = false;
    // printf("%d\n", isPlayershooting(b));
}

// PUBLIC Networkplayer* getGamestateplayer(Networkgamestate *a, int n)
// {
//     return &(*a)->player[n];
// }

// PUBLIC Networkgamestate createNetworkgamestate()
// {
//     Networkgamestate a;
//     for (int i = 0; i < MAX_PLAYERS; i++) a.players[i] = createNetworkplayer(-1);
//     // sizeof(*a);
//     return a;
// }

PUBLIC void* getNetworkgamestateplayer(Networkgamestate *a, int playerID)
{
    return &(*a)->aPlayer[playerID];
}

PUBLIC void killNetworkgamestateplayer(Networkgamestate *a, int n)
{
    (*a)->aPlayer[n].isAlive = false;
}

PUBLIC void reviveNetworkgamestateplayer(Networkgamestate *a, int n)
{
    (*a)->aPlayer[n].isAlive = true;
}

PUBLIC bool isNetworkplayeractive(Networkgamestate *a, int n)
{
    return (*a)->aPlayer[n].isActive;
}

PUBLIC void freeNetworkgamestateplayer(Networkgamestate *a, int n)
{
    (*a)->aPlayer[n].isActive = false;
}

PUBLIC void activateNetworkgamestateplayer(Networkgamestate *a, int n)
{
    (*a)->aPlayer[n].isActive = true;
}

PUBLIC void setGamestateplayer(Networkgamestate *a, void *player, int n)
{
    (*a)->aPlayer[n] = *(Networkplayer*)(player);
}

PUBLIC Uint32 getNetworkplayersize()
{
    return sizeof(struct NetworkPlayer_type);
}

PUBLIC int getNetworkgamestateplayerDirection(Networkgamestate *a, int n)
{
    return (*a)->aPlayer[n].direction;
}

PUBLIC bool isNetworkgamestateplayerAlive(Networkgamestate *a, int n)
{
    return (*a)->aPlayer[n].isAlive;
}

PUBLIC bool isNetworkgamestateplayerShooting(Networkgamestate *a, int n)
{
    return (*a)->aPlayer[n].isShooting;
}

PUBLIC int getNetworkgamestateplayerXtarget(Networkgamestate *a, int n)
{
    return (*a)->aPlayer[n].xTarget;
}

PUBLIC int getNetworkgamestateplayerYtarget(Networkgamestate *a, int n)
{
    return (*a)->aPlayer[n].yTarget;
}

PUBLIC bool iisplayershoot(void *player)
{
    return (Networkplayer*)(player);
}

PUBLIC void setNetworkplayeralive(Networkgamestate *a, int n, bool alive)
{
    (*a)->aPlayer[n].isAlive = alive;
}