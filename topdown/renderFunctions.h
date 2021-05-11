#ifndef RENDERFUNCTIONS_H
#define RENDERFUNCTIONS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "bullet.h"
#include "player.h"
#include "sdlinclude.h"
#include "world.h"


void renderGame(SDL_Renderer* renderer, SDL_Texture* mTiles, SDL_Rect gTiles[], Bullet bullets[][MAX_BULLETS],
    SDL_Texture* bulletTexture, Player players[], SDL_Texture* playerText, SDL_Rect playerRect[], SDL_Point* playerRotationPoint,
    SDL_Texture* gunFireTexture, SDL_Rect gunFireRect, SDL_Texture* explosionTexture, SDL_Rect explosionRect,
    SDL_Point* muzzleRotationPoint, SDL_Texture* gunFireTexture2, SDL_Rect gunFireRect2, SDL_Point* bulletRotationPoint, Mix_Chunk* sound);

// void renderGame(SDL_Renderer* renderer, SDL_Texture* mTiles, SDL_Rect gTiles[], Bullet bullets[],
//     SDL_Texture* bulletTexture, Player players[], SDL_Texture* playerText, SDL_Rect playerRect[], SDL_Point* playerRotationPoint,
//     SDL_Texture* gunFireTexture, SDL_Rect gunFireRect, SDL_Texture* explosionTexture, SDL_Rect explosionRect, 
//     SDL_Point* muzzleRotationPoint, SDL_Texture* gunFireTexture2, SDL_Rect gunFireRect2, SDL_Point* bulletRotationPoint, Mix_Chunk* sound);

// void renderGame(SDL_Renderer* renderer, SDL_Texture* mTiles, SDL_Rect gTiles[], Bullet bullets[], SDL_Texture* bulletTexture, Player players[], SDL_Texture* playerText, SDL_Rect playerRect[], SDL_Point* playerRotationPoint);

// void renderGame(SDL_Renderer* renderer, SDL_Texture* mTiles, SDL_Rect gTiles[], Bullet bullets[][MAX_BULLETS], SDL_Texture* bulletTexture, Player players[], SDL_Texture* playerText, SDL_Rect playerRect[], SDL_Point* playerRotationPoint);
#endif  // RENDERFUNCTIONS_H