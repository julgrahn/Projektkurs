#include "renderFunctions.h"
#include <stdlib.h>

#define PUBLIC /* empty */
#define PRIVATE static

#define HEALTHBARLENGTH 130

PUBLIC void renderGame(SDL_Renderer* renderer, SDL_Texture* mTiles, SDL_Rect gTiles[], Bullet bullets[][MAX_BULLETS],
    SDL_Texture* bulletTexture, Player players[], SDL_Texture* playerText, SDL_Rect playerRect[], SDL_Point* playerRotationPoint,
    SDL_Texture* gunFireTexture, SDL_Rect gunFireRect, SDL_Texture* explosionTexture, SDL_Rect explosionRect,
    SDL_Point* muzzleRotationPoint, SDL_Texture* bloodTexture, SDL_Rect bloodRect, Mix_Chunk* sound,
    SDL_Rect explosionTiles[], SDL_Rect bloodTiles[], Mix_Chunk* soundWall, Mix_Chunk* soundDeath, SDL_Texture* bloodTexture2, SDL_Rect bloodTiles2[])
{
    SDL_RenderClear(renderer);
    // Render Background
    SDL_Rect position;
    position.y = 0;
    position.x = 0;
    position.h = getTileHeight();
    position.w = getTileWidth();

    for (int i = 0; i < getTileRows(); i++)
    {
        for (int j = 0; j < getTileColumns(); j++)
        {
            position.y = i * getTileHeight();
            position.x = j * getTileWidth();
            SDL_RenderCopy(renderer, mTiles, &gTiles[getTileGrid(i, j)], &position);
            // SDL_RenderCopyEx(renderer, mTiles, &gTiles[getTileGrid(i, j)], &position, 0, NULL, SDL_FLIP_NONE);
        }
    }
    // Render Bullets
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        for (int j = 0; j < MAX_BULLETS; j++)
        {
            if (isBulletActive(bullets[i][j]))
            {
                SDL_RenderCopy(renderer, explosionTexture, &explosionTiles[12], getBulletRect(bullets[i][j]));
                // SDL_RenderCopyEx(renderer, explosionTexture, &explosionTiles[12], getBulletRect(bullets[i][j]), 0 , NULL, SDL_FLIP_NONE);
            }
        }
    }
    // Render Players

    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (isPlayerAlive(players[i]))
        {
            SDL_RenderCopyEx(renderer, playerText, &playerRect[getPlayerFrame(players[i])], getPlayerRect(players[i]), getPlayerDirection(players[i]), playerRotationPoint, SDL_FLIP_NONE);
        }
        else // dödsljud fungerar ej optimalt än
        {
            if (checkKilled(players[i]))
            {
                Mix_PlayChannel(-1, soundDeath, 0);
                printf("killed1\n");
            }
        }

    }
    // Render Gunfire
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        for (int j = 0; j < MAX_BULLETS; j++)
        {
            if (bulletHit(bullets[i][j]))
            {
                gunFireRect.x = getBulletX(bullets[i][j]) - 14 - 2; // gunfireRect bör bytas till en annan då denna avser muzzle från vapnet. 
                gunFireRect.y = getBulletY(bullets[i][j]) - 16 - 2;
                if (getWallCollisionBullet(getBulletX(bullets[i][j]) - 2, getBulletY(bullets[i][j]) - 2, 4, 4))
                {
                    if (getBulletHitValue(bullets[i][j]) > 12)
                    {
                        SDL_RenderCopyEx(renderer, explosionTexture, &explosionTiles[45], &gunFireRect, 0, NULL, SDL_FLIP_NONE);
                        Mix_PlayChannel(-1, soundWall, 0); // wall sound
                    }
                    else if (getBulletHitValue(bullets[i][j]) > 9)
                    {
                        SDL_RenderCopyEx(renderer, explosionTexture, &explosionTiles[34], &gunFireRect, 0, NULL, SDL_FLIP_NONE);
                    }
                    else if (getBulletHitValue(bullets[i][j]) > 6)
                    {
                        SDL_RenderCopyEx(renderer, explosionTexture, &explosionTiles[23], &gunFireRect, 0, NULL, SDL_FLIP_NONE);
                    }
                    else if (getBulletHitValue(bullets[i][j]) > 3)
                    {
                        SDL_RenderCopyEx(renderer, explosionTexture, &explosionTiles[45], &gunFireRect, 0, NULL, SDL_FLIP_NONE);
                    }
                }
                // else traff på spelare, visa blod. 
                else
                {
                    if (getBulletOriginX(bullets[i][j]) < gunFireRect.x) // Skott kommer från vanster, anvander blood-right.png
                    {
                        gunFireRect.x += 8; // justering av skott i x-led så att blod kommer battre fran kroppen. 
                        
                        if (getBulletHitValue(bullets[i][j]) > 12)
                        {
                            SDL_RenderCopyEx(renderer, bloodTexture, &bloodTiles[2], &gunFireRect, (getBulletDirection(bullets[i][j]) * 180 / M_PI), muzzleRotationPoint, SDL_FLIP_NONE);
                            // ljud vid träff på spelare
                            //Mix_PlayChannel(-1, soundDeath, 0); // 
                        }
                        else if (getBulletHitValue(bullets[i][j]) > 9)
                        {
                            SDL_RenderCopyEx(renderer, bloodTexture, &bloodTiles[3], &gunFireRect, (getBulletDirection(bullets[i][j]) * 180 / M_PI), muzzleRotationPoint, SDL_FLIP_NONE);
                        }
                        else if (getBulletHitValue(bullets[i][j]) > 6)
                        {
                            SDL_RenderCopyEx(renderer, bloodTexture, &bloodTiles[4], &gunFireRect, (getBulletDirection(bullets[i][j]) * 180 / M_PI), muzzleRotationPoint, SDL_FLIP_NONE);
                        }
                        else if (getBulletHitValue(bullets[i][j]) > 3)
                        {
                            SDL_RenderCopyEx(renderer, bloodTexture, &bloodTiles[7], &gunFireRect, (getBulletDirection(bullets[i][j]) * 180 / M_PI), muzzleRotationPoint, SDL_FLIP_NONE);
                        }
                    }
                    else // Skott kommer från Hoger, anvander blood-left.png
                    {
                        
                        if (getBulletHitValue(bullets[i][j]) > 12)
                        {
                            SDL_RenderCopyEx(renderer, bloodTexture2, &bloodTiles2[2], &gunFireRect, (getBulletDirection(bullets[i][j]) * 180 / M_PI), muzzleRotationPoint, SDL_FLIP_HORIZONTAL && SDL_FLIP_VERTICAL);
                            // ljud vid träff på spelare
                            //Mix_PlayChannel(-1, soundDeath, 0); // 
                        }
                        else if (getBulletHitValue(bullets[i][j]) > 9)
                        {
                            SDL_RenderCopyEx(renderer, bloodTexture2, &bloodTiles2[3], &gunFireRect, (getBulletDirection(bullets[i][j]) * 180 / M_PI), muzzleRotationPoint, SDL_FLIP_HORIZONTAL && SDL_FLIP_VERTICAL);
                        }
                        else if (getBulletHitValue(bullets[i][j]) > 6)
                        {
                            SDL_RenderCopyEx(renderer, bloodTexture2, &bloodTiles2[4], &gunFireRect, (getBulletDirection(bullets[i][j]) * 180 / M_PI), muzzleRotationPoint, SDL_FLIP_HORIZONTAL && SDL_FLIP_VERTICAL);
                        }
                        else if (getBulletHitValue(bullets[i][j]) > 3)
                        {
                            SDL_RenderCopyEx(renderer, bloodTexture2, &bloodTiles2[7], &gunFireRect, (getBulletDirection(bullets[i][j]) * 180 / M_PI), muzzleRotationPoint, SDL_FLIP_HORIZONTAL && SDL_FLIP_VERTICAL);
                        }
                    }
                }
            }
            if (bulletShot(bullets[i][j]))
            {
                gunFireRect.x = getBulletOriginX(bullets[i][j]) - 14;
                gunFireRect.y = getBulletOriginY(bullets[i][j]) - 16;
                SDL_RenderCopyEx(renderer, gunFireTexture, NULL, &gunFireRect, getPlayerDirection(players[i]), muzzleRotationPoint, SDL_FLIP_NONE);

                if (checkShot(bullets[i][j]))
                    Mix_PlayChannel(-1, sound, 0);
            }
        }
    }
}
    


PUBLIC void renderRoundState(SDL_Renderer* renderer, SDL_Rect aRoundStateRect[], SDL_Texture* roundStateTexture, int roundState)
{
    if (roundState == 0 || roundState == 1)
    {
        if (roundState == 0) SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
        else SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        aRoundStateRect[2].x = 0;
        aRoundStateRect[2].y = WINDOWHEIGHT / 2;
        SDL_RenderFillRect(renderer, &aRoundStateRect[2]);
        SDL_RenderCopy(renderer, roundStateTexture, &aRoundStateRect[roundState], &aRoundStateRect[2]);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    } 
}

PUBLIC void renderScoreScreen(SDL_Renderer* renderer, SDL_Rect aScorerect[], SDL_Texture* scoreTexture, SDL_Rect textrect[], SDL_Texture* texttexture, Player players[])
{
 
    int yPadding = 20;
    aScorerect[2].x = 175;
    aScorerect[2].y = 225;

    SDL_RenderCopy(renderer, scoreTexture, &aScorerect[1], &aScorerect[2]);
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (isPlayerActive(players[i]))
        {
            aScorerect[0].x = aScorerect[2].x;
            aScorerect[0].y = aScorerect[2].y + yPadding;
            SDL_RenderCopy(renderer, texttexture, &textrect[i], &aScorerect[0]);
            aScorerect[0].x = aScorerect[2].x + 84;
            SDL_RenderCopy(renderer, texttexture, &textrect[getPlayerKills(players[i])], &aScorerect[0]);
            yPadding += 20;
        }
    }
}

PUBLIC void renderHUD(SDL_Renderer* renderer, Player player, SDL_Rect textrect[], SDL_Texture *texttecture, SDL_Rect *healthbar, SDL_Rect *realoadTimer)
{
    SDL_RenderCopy(renderer, texttecture, &textrect[getPlayerweaponMag(player)/10], &textrect[3]);
    SDL_RenderCopy(renderer, texttecture, &textrect[getPlayerweaponMag(player)%10], &textrect[4]);
    SDL_RenderCopy(renderer, texttecture, &textrect[10], &textrect[2]);
    SDL_RenderCopy(renderer, texttecture, &textrect[2], &textrect[0]);
    SDL_RenderCopy(renderer, texttecture, &textrect[0], &textrect[1]);

    SDL_RenderCopy(renderer, texttecture, &textrect[getPlayerlives(player)], &textrect[7]);

    realoadTimer->h = round(((double)(90-getPlayerReloadprogress(player))/90)*22*(getPlayerReloadprogress(player)>0));
    realoadTimer->y = 24 - round(((double)(90-getPlayerReloadprogress(player))/90)*22*(getPlayerReloadprogress(player)>0));
    SDL_SetRenderDrawColor(renderer,255,255,255,200);
    SDL_RenderFillRect(renderer, realoadTimer);
    
    healthbar->h = 14, healthbar->w = HEALTHBARLENGTH+4, healthbar->x = 98, healthbar->y = 5;
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderDrawRect(renderer, healthbar);
    healthbar->h = 12, healthbar->w = HEALTHBARLENGTH+2, healthbar->x = 99, healthbar->y = 6;
    SDL_RenderDrawRect(renderer, healthbar);
    healthbar->h = 10, healthbar->x = 100, healthbar->y = 7;
    healthbar->w = ((double)getPlayerHealth(player)/100)*HEALTHBARLENGTH;

    SDL_SetRenderDrawColor(renderer,255,0,0,175);
    SDL_RenderFillRect(renderer, healthbar);
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
}

PUBLIC void renderMenu(SDL_Renderer* renderer, SDL_Texture* connectTextures[], SDL_Texture* hostTextures[], SDL_Texture* quitTextures[], Button buttons[], int mouseX, int mouseY, bool shooting)
{

    SDL_RenderClear(renderer);

    // Connect button
    if (mouseX >= (WINDOWWIDTH / 2) - 64 && mouseX <= (WINDOWWIDTH / 2) + 64)
    {
        SDL_RenderCopy(renderer, connectTextures[0], NULL, getButtonRect(buttons[0]));
        SDL_RenderCopy(renderer, hostTextures[0], NULL, getButtonRect(buttons[1]));
        SDL_RenderCopy(renderer, quitTextures[0], NULL, getButtonRect(buttons[2]));

        // Connect button

        if (mouseY > 100 && mouseY < 164)
        {
            SDL_RenderCopy(renderer, connectTextures[1], NULL, getButtonRect(buttons[0]));
        }

        if (mouseY > 100 && mouseY < 164 && shooting)
        {

            SDL_RenderCopy(renderer, connectTextures[2], NULL, getButtonRect(buttons[0]));
        }

        // Host button

        if (mouseY > 200 && mouseY < 264)
        {
            SDL_RenderCopy(renderer, hostTextures[1], NULL, getButtonRect(buttons[1]));
        }

        if (mouseY > 200 && mouseY < 264 && shooting)
        {

            SDL_RenderCopy(renderer, hostTextures[2], NULL, getButtonRect(buttons[1]));
        }

        // Quit button

        if (mouseY > 300 && mouseY < 364)
        {
            SDL_RenderCopy(renderer, quitTextures[1], NULL, getButtonRect(buttons[2]));
        }

        if (mouseY > 300 && mouseY < 364 && shooting)
        {

            SDL_RenderCopy(renderer, quitTextures[2], NULL, getButtonRect(buttons[2]));
        }
    }
    else
    {
        SDL_RenderCopy(renderer, connectTextures[0], NULL, getButtonRect(buttons[0]));
        SDL_RenderCopy(renderer, hostTextures[0], NULL, getButtonRect(buttons[1]));
        SDL_RenderCopy(renderer, quitTextures[0], NULL, getButtonRect(buttons[2]));
    }

    SDL_RenderPresent(renderer);
}

PUBLIC void newRound(Mix_Chunk* roundStartSound)
{
    resetTileGridMap();
    Mix_PlayChannel(-1, roundStartSound, 0);
}