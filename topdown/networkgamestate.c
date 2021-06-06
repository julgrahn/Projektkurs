#include "networkgamestate.h"

#define PUBLIC

typedef struct NetworkBullet_type{
	short xPos, yPos, angle;
	Uint8 damage;
	Uint8 control_active;
}Networkbullet;

typedef struct NetworkPlayer_type{
	short direction;
	short posX, posY;
	Uint8 status;   // bit: 2 - alive, bit: 1 - active, bit: 0 - invulnerable
	Sint8 lives, health, kills;
<<<<<<< Updated upstream
	bool killed;//tillfälligt för dödsljud
=======
>>>>>>> Stashed changes
	Networkbullet aBullet[MAX_BULLETS];
	Uint32 wallState[27];
}Networkplayer;

struct Networkgamestate_type {
	Networkplayer aPlayer[5];
	Uint8 roundState;
	uint8_t createWalls;
};

PUBLIC Networkgamestate createNetworkgamestate()
{
	Networkgamestate a = malloc(sizeof(struct Networkgamestate_type));
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		a->aPlayer[i].direction = 0;
		a->aPlayer[i].posX = 0, a->aPlayer[i].posY = 0;
		a->aPlayer[i].health = 100;
		a->aPlayer[i].lives = 0;
		a->aPlayer[i].status = 0;
<<<<<<< Updated upstream
		a->aPlayer[i].killed = false;
=======
>>>>>>> Stashed changes
		a->aPlayer[i].kills = 0;
		for (int j = 0; j < MAX_BULLETS; j++)
		{
			a->aPlayer[i].aBullet[j].control_active = 0;
		}
	}
	a->roundState = 0;
	a->createWalls = 0;

	return a;
}

PUBLIC Uint32 getGamestatesize()
{
	return sizeof(struct Networkgamestate_type);
}

PUBLIC void setRoundState(Networkgamestate a, int newValue)
{
	a->roundState = newValue;
}

PUBLIC Uint8 getRoundState(Networkgamestate a)
{
	return a->roundState;
}

PUBLIC int getNetPlayerX(Networkgamestate a, int n)
{
	return a->aPlayer[n].posX;
}

PUBLIC int getNetPlayerY(Networkgamestate a, int n)
{
	return a->aPlayer[n].posY;
}

PUBLIC void setNetPlayerKills(Networkgamestate a, int n, int newValue)
{
	a->aPlayer[n].kills = newValue;
}

PUBLIC void setNetPlayer(Networkgamestate a, int n, Player b)
{
	a->aPlayer[n].posX = getPlayerX(b);
	a->aPlayer[n].posY = getPlayerY(b);
	a->aPlayer[n].direction = (short)getPlayerDirection(b);
	a->aPlayer[n].status |= 0b010;
}

PUBLIC void* getNetPlayer(Networkgamestate a, int playerID)
{
	return &a->aPlayer[playerID];
}

PUBLIC void killNetPlayer(Networkgamestate a, int n)
{
	a->aPlayer[n].status &= 0b011;
}

PUBLIC void reviveNetPlayer(Networkgamestate a, int n)
{
	a->aPlayer[n].health = 100;
	a->aPlayer[n].status |= 0b100;
}

PUBLIC int getNetPlayerKills(Networkgamestate a, int n)
{
	return a->aPlayer[n].kills;
}

PUBLIC bool isNetPlayerActive(Networkgamestate a, int n)
{
	return (a->aPlayer[n].status & 0b010) >> 1; 
}

PUBLIC void freeNetPlayer(Networkgamestate a, int n)
{
	a->aPlayer[n].status &= 0b101;
	a->aPlayer[n].kills = 0;
}

PUBLIC void activateNetPlayer(Networkgamestate a, int n)
{
	a->aPlayer[n].status |= 0b010;
}

PUBLIC Uint32 getNetPlayerSize()
{
	return sizeof(struct NetworkPlayer_type);
}

PUBLIC int getNetPlayerDirection(Networkgamestate a, int n)
{
	return a->aPlayer[n].direction;
}

PUBLIC bool isNetPlayerAlive(Networkgamestate a, int n)
{
	return (a->aPlayer[n].status & 0b100) >> 2;
}

PUBLIC void setNetPlayerAlive(Networkgamestate a, int n, bool alive)
{
	a->aPlayer[n].status = (a->aPlayer[n].status |= 0b100) * alive + (a->aPlayer[n].status &= 0b011) * !alive;
}

PUBLIC void setNetBullets(Networkgamestate a, int playerID, Bullet bullets[])
{
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		a->aPlayer[playerID].aBullet[i].control_active = (a->aPlayer[playerID].aBullet[i].control_active |= 0b01) * isBulletActive(bullets[i]) + (a->aPlayer[playerID].aBullet[i].control_active &= 0b10) * !isBulletActive(bullets[i]);
		a->aPlayer[playerID].aBullet[i].angle = getBulletDirection(bullets[i])*10000;
		a->aPlayer[playerID].aBullet[i].xPos = getBulletX(bullets[i]);
		a->aPlayer[playerID].aBullet[i].yPos = getBulletY(bullets[i]);
		a->aPlayer[playerID].aBullet[i].damage = getBulletDamage(bullets[i]);
	}
}

PUBLIC int getNetBulletX(Networkgamestate a, int playerID, int bulletNo)
{
	return a->aPlayer[playerID].aBullet[bulletNo].xPos;
}

PUBLIC int getNetBulletY(Networkgamestate a, int playerID, int bulletNo)
{
	return a->aPlayer[playerID].aBullet[bulletNo].yPos;
}

PUBLIC bool isNetbulletActive(Networkgamestate a, int playerID, int bulletNo)
{
	return a->aPlayer[playerID].aBullet[bulletNo].control_active & 0b01;
}

PUBLIC void damageNetplayer(Networkgamestate a, int playerID, int damage, int shooterID)
{
	a->aPlayer[playerID].health -= damage;
	if(a->aPlayer[playerID].health <= 0)
	{
<<<<<<< Updated upstream
		// a->aPlayer[playerID].killed = true;//tillfälligt för dödsljud
=======
>>>>>>> Stashed changes
		if(--a->aPlayer[playerID].lives < 0) a->aPlayer[playerID].lives = 0;
		killNetPlayer(a, playerID);
		a->aPlayer[shooterID].kills++;
	}
}

PUBLIC int getNetplayerHealth(Networkgamestate a, int playerID)
{
	return a->aPlayer[playerID].health;
}

PUBLIC bool isNetplayerInvulnerable(Networkgamestate a, int playerID)
{
	return (a->aPlayer[playerID].status & 0b001);
}

PUBLIC void setNetplayerInvulnerable(Networkgamestate a, int playerID, bool value)
{
	a->aPlayer[playerID].status = (a->aPlayer[playerID].status |= 0b001) * value + (a->aPlayer[playerID].status &= 0b110) * !value;
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
	a->aPlayer[playerID].aBullet[bulletNo].control_active = 0b10;
}

PUBLIC void setNetplayerPos(Networkgamestate a, int playerID, int x, int y)
{
	a->aPlayer[playerID].posX = x, a->aPlayer[playerID].posY = y;
}

PUBLIC bool netbulletStatus(Networkgamestate a, int playerID, int bulletID)
{
	return (a->aPlayer[playerID].aBullet[bulletID].control_active & 0b10)>>1;
}

PUBLIC void netBulletclearcontrol(Networkgamestate a, int playerID, int bulletID)
{
	a->aPlayer[playerID].aBullet[bulletID].control_active &= 0b01;
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


<<<<<<< Updated upstream
// för dödsljud
PUBLIC bool getNetPlayerKilled(Networkgamestate a, int playerID)
{
	return a->aPlayer[playerID].killed;
}

PUBLIC void resetPlayerKilled(Networkgamestate a, int playerID)
{
	if(a->aPlayer[playerID].killed){
		printf("skicka paket\n");
	}
	a->aPlayer[playerID].killed = false;
}

=======
>>>>>>> Stashed changes
PUBLIC uint32_t* getWallState(Networkgamestate a, int playerID)
{
	return a->aPlayer[playerID].wallState;
}

PUBLIC void combineWallstates(Networkgamestate a, uint32_t wallstates[])
{
	for (int i = 0; i < 27; i++)
	{
		wallstates[i] &= a->aPlayer[0].wallState[i];
		wallstates[i] &= a->aPlayer[1].wallState[i];
		wallstates[i] &= a->aPlayer[2].wallState[i];
		wallstates[i] &= a->aPlayer[3].wallState[i];
		wallstates[i] &= a->aPlayer[4].wallState[i];
		a->aPlayer[0].wallState[i] = a->aPlayer[1].wallState[i] = a->aPlayer[2].wallState[i] = a->aPlayer[3].wallState[i] = a->aPlayer[4].wallState[i] = wallstates[i];
	}
}

PUBLIC void resetWallStates(Networkgamestate a, uint32_t wallstates[])
{
	 for (int i = 0; i < 27; i++)
	{
		a->aPlayer[0].wallState[i] = wallstates[i];
		a->aPlayer[1].wallState[i] = wallstates[i];
		a->aPlayer[2].wallState[i] = wallstates[i];
		a->aPlayer[3].wallState[i] = wallstates[i];
		a->aPlayer[4].wallState[i] = wallstates[i];
	}
}

PUBLIC void createWalls(Networkgamestate a, bool b)
{
	a->createWalls = b;
}