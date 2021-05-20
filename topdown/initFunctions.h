#ifndef INITFUNCTIONS_H
#define INITFUNCTIONS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "bullet.h"
#include "player.h"
#include "sdlinclude.h"
#include "world.h"

void initGameObjects(Player players[], Bullet bullets[][MAX_BULLETS]);
bool initSDL(SDL_Renderer** renderer, Mix_Chunk** sound, Mix_Chunk** soundWall, Mix_Chunk** soundDeath);
void initClient(UDPsocket* sd, UDPpacket** p, UDPpacket** p2);
void initGameHUD(SDL_Renderer* renderer, SDL_Rect textRect[], SDL_Texture** textTexture, SDL_Rect* healthBar, SDL_Rect* reloadTimer, SDL_Rect aScorerect[], SDL_Texture** scoreTexture, SDL_Rect aRoundStateRect[], SDL_Texture** roundStateTexture);
void loadMenu(SDL_Renderer* renderer, SDL_Texture* connectTextures[], SDL_Texture* hostTexture[], SDL_Texture* quitTexture[]);
void loadMedia(SDL_Renderer* renderer, SDL_Rect gTiles[], SDL_Texture** tiles, SDL_Rect playerRect[], 
                        SDL_Texture** pTexture, SDL_Cursor** cursor, SDL_Texture** bulletTexture, 
                        SDL_Texture** gunFireTexture, SDL_Texture** explosionTexture, 
                        SDL_Texture** bloodTexture, Mix_Chunk** sound,
                        SDL_Rect explosionTiles[], SDL_Rect bloodTiles[], Mix_Chunk** soundWall, Mix_Chunk** soundDeath, SDL_Texture** bloodTexture2,  SDL_Rect bloodTiles2[], Mix_Chunk** prepareToFight);
                
                        
#endif  // INITFUNCTIONS_H