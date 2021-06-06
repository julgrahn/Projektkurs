#ifndef WAEPON_H
#define WAEPON_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct Weapon_type* Weapon;

Weapon createWeapon();
int getWeapondamage(Weapon a);
int getMag(Weapon a);
int getReloadprogress(Weapon a);

void weaponTick(Weapon a);
void resetWeapon(Weapon a);
void reloadWeapon(Weapon a);

bool fireWeapon(Weapon a);

#endif
