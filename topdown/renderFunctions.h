#ifndef RENDERFUNCTIONS_H
#define RENDERFUNCTIONS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "bullet.h"
#include "player.h"
#include "sdlinclude.h"
#include "world.h"
#include "button.h"

void renderHUD(SDL_Renderer* renderer, Player player, SDL_Rect textrect[], SDL_Texture *texttecture);

void renderGame(SDL_Renderer* renderer, SDL_Texture* mTiles, SDL_Rect gTiles[], Bullet bullets[][MAX_BULLETS],
    SDL_Texture* bulletTexture, Player players[], SDL_Texture* playerText, SDL_Rect playerRect[],
    SDL_Texture* gunFireTexture, SDL_Texture* explosionTexture, SDL_Texture* bloodTexture, Mix_Chunk* sound,
    SDL_Rect explosionTiles[], SDL_Rect bloodTiles[], Mix_Chunk* soundWall, Mix_Chunk* soundDeath);
    
void renderRoundState(SDL_Renderer* renderer, SDL_Rect aRoundStateRect[], SDL_Texture* roundStateTexture, int roundState, int winner, SDL_Rect textrect[], SDL_Texture* texttextur);
void renderScoreScreen(SDL_Renderer* renderer, SDL_Rect aScorerect[], SDL_Texture* scoreTexture, SDL_Rect textrect[], SDL_Texture* texttecture, Player players[]);
// void renderGame(SDL_Renderer* renderer, SDL_Texture* mTiles, SDL_Rect gTiles[], Bullet bullets[][MAX_BULLETS],
//     SDL_Texture* bulletTexture, Player players[], SDL_Texture* playerText, SDL_Rect playerRect[], SDL_Point* playerRotationPoint,
//     SDL_Texture* gunFireTexture, SDL_Rect gunFireRect, SDL_Texture* explosionTexture, SDL_Rect explosionRect, 
//     SDL_Point* muzzleRotationPoint, SDL_Texture* bloodTexture, SDL_Rect bloodRect, Mix_Chunk* sound,
//     SDL_Rect explosionTiles[], SDL_Rect bloodTiles[], Mix_Chunk* soundWall, Mix_Chunk* soundDeath);
void newRound(Mix_Chunk* roundStartSound);

void renderMenu(SDL_Renderer* renderer, SDL_Texture* connectTextures[], SDL_Texture* hostTextures[], SDL_Texture* quitTextures[], 
    Button buttons[], SDL_Texture* backgroundTexture, int mouseX, int mouseY, bool shooting);

void renderGame2(SDL_Renderer* renderer, SDL_Texture* mTiles, SDL_Rect gTiles[], Bullet bullets[][MAX_BULLETS],
    SDL_Texture* bulletTexture, Player players[], SDL_Texture* playerText[], SDL_Texture* playerFeetText[], SDL_Texture* reloadText[20], SDL_Texture* idleText[20], SDL_Texture* shootText[],
    SDL_Texture* gunFireTexture, SDL_Texture* explosionTexture, SDL_Texture* bloodTexture, Mix_Chunk* sound,
    SDL_Rect explosionTiles[], SDL_Rect bloodTiles[], Mix_Chunk* soundWall, Mix_Chunk* soundDeath);

#endif  // RENDERFUNCTIONS_H