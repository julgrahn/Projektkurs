#ifndef MENU_H
#define MENU_H

#include "world.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "sdlinclude.h"
#include "bullet.h"
#include "player.h"

bool initMenu(SDL_Renderer* renderMenu);
void loadMenu(SDL_Renderer* renderMenu, SDL_Texture** buttons, SDL_Cursor** cursor);



#endif MENU_H