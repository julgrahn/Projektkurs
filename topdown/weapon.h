#ifndef WAEPON_H
#define WAEPON_H
#include "sdlinclude.h"
#include <stdbool.h>

typedef struct Weapon_type* Weapon;

Weapon createWeapon();
bool fireWeapon(Weapon a);
void weaponTick(Weapon a);
int getWeapondamage(Weapon a);
int getMag(Weapon a);
int getReloadprogress(Weapon a);
void resetWeapon(Weapon a);
void reloadWeapon(Weapon a);

#endif
