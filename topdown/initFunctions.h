#ifndef INITFUNCTIONS_H
#define INITFUNCTIONS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "bullet.h"
#include "player.h"
#include "sdlinclude.h"
#include "world.h"

bool initSDL(SDL_Renderer** renderer);
void initGameObjects(Player players[], Bullet bullets[][MAX_BULLETS]);
// bool initSDL(SDL_Renderer** renderer, Mix_Chunk** sound);
// void initGameObjects(Player players[], Bullet bullets[]);
void initClient(UDPsocket* sd, UDPpacket** p, UDPpacket** p2);
void loadMedia(SDL_Renderer* renderer, SDL_Rect gTiles[], SDL_Texture** tiles,
                SDL_Rect playerRect[], SDL_Texture** pTexture, SDL_Cursor** cursor, 
                SDL_Texture** bulletTexture, SDL_Texture** gunFireTexture, 
                SDL_Rect gunFireRect, SDL_Texture** explosionTexture, SDL_Rect explosionRect, 
                SDL_Texture** gunFireTexture2, SDL_Rect gunFireRect2, Mix_Chunk** sound);

#endif  // INITFUNCTIONS_H