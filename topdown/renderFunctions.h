#ifndef RENDERFUNCTIONS_H
#define RENDERFUNCTIONS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "bullet.h"
#include "player.h"
#include "sdlinclude.h"
#include "world.h"

// void renderGame(SDL_Renderer* renderer, SDL_Texture* mTiles, SDL_Rect gTiles[], Bullet bullets[], SDL_Texture* bulletTexture, Player players[], SDL_Texture* playerText, SDL_Rect playerRect[], SDL_Point* playerRotationPoint);

void renderGame(SDL_Renderer* renderer, SDL_Texture* mTiles, SDL_Rect gTiles[], Bullet bullets[][MAX_BULLETS], SDL_Texture* bulletTexture, Player players[], SDL_Texture* playerText, SDL_Rect playerRect[], SDL_Point* playerRotationPoint);
#endif  // RENDERFUNCTIONS_H