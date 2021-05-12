#include "networkgamestate.h"

#define PUBLIC
#define NETBULLETDMG 10
typedef struct NetworkBullet_type{
	short xPos, yPos, angle;
    Uint8 damage;
    bool control;
    bool active;
}Networkbullet;

typedef struct NetworkPlayer_type{
    short direction, health;
    short posX, posY;// xTarget, yTarget;
    bool isAlive, isActive, invulnerable; //isShooting;
    // Uint8 status;
    short lives;
    // Uint8 damage;
    // Uint8 playerstatus, damage, health;
    // short bullets[15][4];
    Networkbullet aBullet[MAX_BULLETS];
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
        a->aPlayer[i].isActive = false;
        a->aPlayer[i].health = 100;
        a->aPlayer[i].lives = 0;
        a->aPlayer[i].invulnerable = false;
        for (int j = 0; j < MAX_BULLETS; j++)
        {
            a->aPlayer[i].aBullet[j].active = false;
            a->aPlayer[i].aBullet[j].control = false;
        }
    }
    return a;
}


PUBLIC Uint32 getGamestatesize()
{
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
    (*a)->aPlayer[n].posX = getPlayerX(b);
    (*a)->aPlayer[n].posY = getPlayerY(b);
    (*a)->aPlayer[n].direction = (short)getPlayerDirection(b);
    (*a)->aPlayer[n].isActive = true;
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

PUBLIC void killNetworkplayer(Networkgamestate *a, int n)
{
    (*a)->aPlayer[n].isAlive = false;
}

PUBLIC void reviveNetworkgamestateplayer(Networkgamestate *a, int n)
{
    (*a)->aPlayer[n].isAlive = true;
    (*a)->aPlayer[n].health = 100;
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

PUBLIC bool isNetworkplayerAlive(Networkgamestate *a, int n)
{
    return (*a)->aPlayer[n].isAlive;
}

// PUBLIC bool isNetworkgamestateplayerShooting(Networkgamestate *a, int n)
// {
//     return (*a)->aPlayer[n].isShooting;
// }


PUBLIC void setNetworkplayeralive(Networkgamestate *a, int n, bool alive)
{
    (*a)->aPlayer[n].isAlive = alive;
}

PUBLIC void setNetworkbullets(Networkgamestate a, int playerID, Bullet bullets[])
{
    int i;
    for (i = 0; i < MAX_BULLETS; i++)
    {
        a->aPlayer[playerID].aBullet[i].active = isBulletActive(bullets[i]);
        a->aPlayer[playerID].aBullet[i].angle = getBulletDirection(bullets[i])*10000;
        a->aPlayer[playerID].aBullet[i].xPos = getBulletX(bullets[i]);
        a->aPlayer[playerID].aBullet[i].yPos = getBulletY(bullets[i]);
        a->aPlayer[playerID].aBullet[i].damage = getBulletDamage(bullets[i]);
    }
}

PUBLIC int getNetbulletX(Networkgamestate a, int playerID, int bulletNo)
{
    return a->aPlayer[playerID].aBullet[bulletNo].xPos;
}

PUBLIC int getNetbulletY(Networkgamestate a, int playerID, int bulletNo)
{
    return a->aPlayer[playerID].aBullet[bulletNo].yPos;
}

PUBLIC bool isNetbulletActive(Networkgamestate a, int playerID, int bulletNo)
{
    return a->aPlayer[playerID].aBullet[bulletNo].active;
}

PUBLIC void damageNetplayer(Networkgamestate a, int playerID, int damage)
{
    a->aPlayer[playerID].health -= damage;
    if(a->aPlayer[playerID].health <= 0)
    {
        a->aPlayer[playerID].lives -= 1;
        killNetworkplayer(&a, playerID);
    }
}

PUBLIC int getNetplayerHealth(Networkgamestate a, int playerID)
{
    return a->aPlayer[playerID].health;
}

PUBLIC bool isNetplayerInvulnerable(Networkgamestate a, int playerID)
{
    return a->aPlayer[playerID].invulnerable;
}

PUBLIC void setNetplayerInvulnerable(Networkgamestate a, int playerID, bool value)
{
    a->aPlayer[playerID].invulnerable = value;
}

PUBLIC int getNetplayerLives(Networkgamestate a, int playerID)
{
    return a->aPlayer[playerID].lives;
}

PUBLIC void setNetplayerHealth(Networkgamestate a, int playerID, int health)
{
    a->aPlayer[playerID].health = health;
}

PUBLIC void setNetplayerLives(Networkgamestate a, int playerID, int lives)
{
    a->aPlayer[playerID].lives = lives;
}

PUBLIC void freeNetbullet(Networkgamestate a, int playerID, int bulletNo)
{
    a->aPlayer[playerID].aBullet[bulletNo].active = false;
    a->aPlayer[playerID].aBullet[bulletNo].control = true;
}

PUBLIC void setNetplayerPos(Networkgamestate a, int playerID, int x, int y)
{
    a->aPlayer[playerID].posX = x, a->aPlayer[playerID].posY = y;
}

PUBLIC bool netbulletStatus(Networkgamestate a, int playerID, int bulletID)
{
    return a->aPlayer[playerID].aBullet[bulletID].control;
}

PUBLIC void netBulletclearcontrol(Networkgamestate a, int playerID, int bulletID)
{
    a->aPlayer[playerID].aBullet[bulletID].control = 0;
}

PUBLIC double getNetbulletspeedX(Networkgamestate a, int playerID, int bulletID)
{
    return BULLET_SPEED * cos((double)a->aPlayer[playerID].aBullet[bulletID].angle/10000);
}

PUBLIC double getNetbulletspeedY(Networkgamestate a, int playerID, int bulletID)
{
    return BULLET_SPEED * sin((double)a->aPlayer[playerID].aBullet[bulletID].angle/10000);
}

PUBLIC double getNetbulletAngle(Networkgamestate a, int playerID, int bulletID)
{
    return (double)a->aPlayer[playerID].aBullet[bulletID].angle/10000;
}

PUBLIC int getNetbulletdamage(Networkgamestate a, int playerID, int bulletID)
{
    return a->aPlayer[playerID].aBullet[bulletID].damage;
}