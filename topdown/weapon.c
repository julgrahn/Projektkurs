#include "weapon.h"

#define PUBLIC
#define PRIVATE static

struct Weapon_type {
    int firerate;
    int damage;
    int magazineSize, magazine;
    int isReady;
    int reload;  
    bool isEmpty;
};

PUBLIC Weapon createWeapon()
{
    Weapon a = malloc(sizeof(struct Weapon_type));
    a->firerate = 10;
    a->isEmpty = false;
    a->magazineSize = a->magazine = 20;
    a->damage = 20;
    a->isReady = 0;
    a->reload = 0;
    return a;
}

PUBLIC void weaponTick(Weapon a)
{
    if(a->isReady)
    {
        a->isReady--;
    }
    if(a->reload)
    {
        a->reload--;
        if(!a->reload)
        {
            a->magazine = a->magazineSize;
        }
    }
}

//Check if weapon can fire then return true
PUBLIC bool fireWeapon(Weapon a)
{
    if(!a->isReady && a->magazine)
    {
        a->isReady = a->firerate;
        a->magazine--;
        return true;
    }
    else{
        if(!a->magazine && !a->reload) a->reload = 90;
        return false;
    }
}

PUBLIC int getWeapondamage(Weapon a)
{
    return a->damage;
}

PUBLIC int getMag(Weapon a)
{
    return a->magazine;
}

PUBLIC int getReloadprogress(Weapon a)
{
    return a->reload;
}

PUBLIC void resetWeapon(Weapon a)
{
    a->magazine = 20;
    a->reload = 0;
    a->isReady = 0;
}

PUBLIC void reloadWeapon(Weapon a)
{
    if(!a->reload && a->magazine != a->magazineSize) { a->magazine = 0; a->reload = 90; }
}