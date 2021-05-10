#include "weapon.h"

#define PUBLIC
#define PRIVATE static

struct Weapon_type {
    int firerate;
    int damage;
    int magazineSize;
    bool isEmpty;
    int isReady;  
};

PUBLIC Weapon createWeapon()
{
    Weapon a = malloc(sizeof(struct Weapon_type));
    a->firerate = 40;
    a->isEmpty = false;
    a->magazineSize = 20;
    a->damage = 5;
    a->isReady = true;
    return a;
}

PUBLIC bool fireWeapon(Weapon a)
{
    if(!a->isReady)
    {
        a->isReady = a->firerate;
        return true;
    }
    else{
        a->isReady--;
        return false;
    }
}