#ifndef INITFUNCTIONS_H
#define INITFUNCTIONS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "bullet.h"
#include "player.h"
#include "sdlinclude.h"
#include "world.h"
#include "button.h"

bool initSDL(SDL_Renderer** renderer);
void initGameObjects(Player players[], Bullet bullets[]);
void initClient(UDPsocket* sd, UDPpacket** p, UDPpacket** p2);
void loadMedia(SDL_Renderer* renderer, SDL_Rect gTiles[], SDL_Texture** tiles, SDL_Rect playerRect[], SDL_Texture** pTexture, SDL_Cursor** cursor, SDL_Texture** bulletTexture);
void loadMenu(SDL_Renderer* renderer, SDL_Texture* connectTexture, SDL_Texture* hostTexture, SDL_Texture* quitTexture);

#endif  // INITFUNCTIONS_H