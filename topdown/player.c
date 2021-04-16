#include <stdlib.h>
#include "player.h"

#define PUBLIC
#define SPEED 1
struct Player_type{
    int health;
    int positionX;
    int positionY;
    int speed;
};

PUBLIC Player createPlayer(int x, int y)
{
    Player a = malloc(sizeof(struct Player_type));
    a->health = 100;
    a->positionX = x;
    a->positionY = y;
    a->speed = SPEED;
    
    return a;
}