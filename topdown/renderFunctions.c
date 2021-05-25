#include "renderFunctions.h"
#include <stdlib.h>

#define PUBLIC /* empty */
#define PRIVATE static

#define HEALTHBARLENGTH 130
#define EXPLOSIONSIZE 35
#define BLOODSPLATTERSIZE 45

PUBLIC void renderGame(SDL_Renderer* renderer, SDL_Texture* mTiles, SDL_Rect gTiles[], Bullet bullets[][MAX_BULLETS],
    SDL_Texture* bulletTexture, Player players[], SDL_Texture* playerText, SDL_Rect playerRect[],
    SDL_Texture* gunFireTexture, SDL_Texture* explosionTexture, SDL_Texture* bloodTexture, Mix_Chunk* sound,
    SDL_Rect explosionTiles[], SDL_Rect bloodTiles[], Mix_Chunk* soundWall, Mix_Chunk* soundDeath)
{
    static SDL_Point playerRotationPoint = { 20, 32 };
    static SDL_Point muzzleRotationPoint = { 14, 16 };
    static SDL_Point bloodRotationPoint = { BLOODSPLATTERSIZE/2, BLOODSPLATTERSIZE/2 };
    static SDL_Rect explosionRect = { 0, 0, EXPLOSIONSIZE, EXPLOSIONSIZE };
    static SDL_Rect bloodRect = { 0, 0, BLOODSPLATTERSIZE, BLOODSPLATTERSIZE };
    static SDL_Rect gunFireRect = { 0, 0, 40, 40 };
    static SDL_Rect position = { 0, 0, TILE_WIDTH, TILE_HEIGHT };
    static SDL_Rect player = { 0, 0, 64, 64 };

    for (int i = 0; i < getTileRows(); i++)
    {
        for (int j = 0; j < getTileColumns(); j++)
        {
            position.y = i * TILE_HEIGHT;
            position.x = j * TILE_WIDTH;
            SDL_RenderCopy(renderer, mTiles, &gTiles[getTileGrid(i, j)], &position);
            // SDL_RenderCopyEx(renderer, mTiles, &gTiles[getTileGrid(i, j)], &position, 0, NULL, SDL_FLIP_NONE);
        }
    }
    // Render Bullets
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        for(int j = 0; j < MAX_BULLETS; j++)
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
            player.x = getPlayerX(players[i])-20, player.y = getPlayerY(players[i])-32;
            SDL_RenderCopyEx(renderer, playerText, &playerRect[getPlayerFrame(players[i])], &player, getPlayerDirection(players[i]), &playerRotationPoint, SDL_FLIP_NONE);
        }
        else // dödsljud fungerar ej optimalt än
        {
            if (checkKilled(players[i]))
            {
                Mix_PlayChannel(-1, soundDeath, 0);
            }
        }
    }
    // Render Muzzleflash, Bullet Hit
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        for (int j = 0; j < MAX_BULLETS; j++)
        {
            if (bulletHit(bullets[i][j]))
            {
                if (getWallCollisionBullet(getBulletX(bullets[i][j])-2, getBulletY(bullets[i][j])-2, 4, 4))
                {
                    explosionRect.x = getBulletX(bullets[i][j]) - explosionRect.w/2;
                    explosionRect.y = getBulletY(bullets[i][j]) - explosionRect.h/2;

                    if (getBulletHitValue(bullets[i][j]) > 12)
                    {
                        SDL_RenderCopyEx(renderer, explosionTexture, &explosionTiles[45], &explosionRect, 0, NULL, SDL_FLIP_NONE);   
                        Mix_PlayChannel(-1, soundWall, 0); // wall sound                    
                    }
                    else if (getBulletHitValue(bullets[i][j]) > 9)
                    {
                        SDL_RenderCopyEx(renderer, explosionTexture, &explosionTiles[34], &explosionRect, 0, NULL, SDL_FLIP_NONE);
                    }
                    else if (getBulletHitValue(bullets[i][j]) > 6)
                    {
                        SDL_RenderCopyEx(renderer, explosionTexture, &explosionTiles[23], &explosionRect, 0, NULL, SDL_FLIP_NONE);
                    }
                    else if (getBulletHitValue(bullets[i][j]) > 3)
                    {
                        SDL_RenderCopyEx(renderer, explosionTexture, &explosionTiles[45], &explosionRect, 0, NULL, SDL_FLIP_NONE);
                    }
                }
                // else traff på spelare, visa blod. 
                else
                {
                    bloodRect.x = getBulletX(bullets[i][j]) - bloodRect.w/2;
                    bloodRect.y = getBulletY(bullets[i][j]) - bloodRect.h/2;
                    if (getBulletHitValue(bullets[i][j]) > 12)
                    {
                        SDL_RenderCopyEx(renderer, bloodTexture, &bloodTiles[9], &bloodRect, (getBulletDirection(bullets[i][j])*180/M_PI), &bloodRotationPoint, SDL_FLIP_HORIZONTAL && SDL_FLIP_VERTICAL);
                        // ljud vid träff på spelare
                        Mix_PlayChannel(-1, soundDeath, 0);
                    }
                    else if (getBulletHitValue(bullets[i][j]) > 9)
                    {
                        SDL_RenderCopyEx(renderer, bloodTexture, &bloodTiles[10], &bloodRect, (getBulletDirection(bullets[i][j])*180/M_PI), &bloodRotationPoint, SDL_FLIP_HORIZONTAL && SDL_FLIP_VERTICAL);
                    }
                    else if (getBulletHitValue(bullets[i][j]) > 6)
                    {
                        SDL_RenderCopyEx(renderer, bloodTexture, &bloodTiles[11], &bloodRect, (getBulletDirection(bullets[i][j])*180/M_PI), &bloodRotationPoint, SDL_FLIP_HORIZONTAL && SDL_FLIP_VERTICAL);
                    }
                    else if (getBulletHitValue(bullets[i][j]) > 3)
                    {
                        SDL_RenderCopyEx(renderer, bloodTexture, &bloodTiles[12], &bloodRect, (getBulletDirection(bullets[i][j])*180/M_PI), &bloodRotationPoint, SDL_FLIP_HORIZONTAL && SDL_FLIP_VERTICAL);
                    }
                }
            }
            if (bulletShot(bullets[i][j]))
            {
                gunFireRect.x = getPlayerGunbarrelX(players[i]) - 14;
                gunFireRect.y = getPlayerGunbarrelY(players[i]) - 16;
                SDL_RenderCopyEx(renderer, gunFireTexture, NULL, &gunFireRect, getPlayerDirection(players[i]), &muzzleRotationPoint, SDL_FLIP_NONE);

                if(checkShot(bullets[i][j]))
                    Mix_PlayChannel(-1, sound, 0);
            }
        }
    }
}

PUBLIC void renderRoundState(SDL_Renderer* renderer, SDL_Rect aRoundStateRect[], SDL_Texture* roundStateTexture, int roundState, int winner, SDL_Rect textrect[], SDL_Texture* texttexture)
{
    if (roundState == 0 || roundState == 1)
    {
        if (roundState == 0) SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
        else SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        aRoundStateRect[3].x = 0;
        aRoundStateRect[3].y = WINDOWHEIGHT / 2;
        SDL_RenderFillRect(renderer, &aRoundStateRect[3]);
        SDL_RenderCopy(renderer, roundStateTexture, &aRoundStateRect[roundState], &aRoundStateRect[3]);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    }
    else if (roundState == 3)
    {        
        aRoundStateRect[3].x = 0;
        aRoundStateRect[3].y = WINDOWHEIGHT / 2;
        
        SDL_RenderCopy(renderer, roundStateTexture, &aRoundStateRect[2], &aRoundStateRect[3]);
        aRoundStateRect[3].x += 900;
        int temp = aRoundStateRect[3].w;
        aRoundStateRect[3].w = 50;
        SDL_RenderCopy(renderer, texttexture, &textrect[winner], &aRoundStateRect[3]);
        aRoundStateRect[3].w = temp;
        aRoundStateRect[3].x -= 900;

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

PUBLIC void renderHUD(SDL_Renderer* renderer, Player player, SDL_Rect textrect[], SDL_Texture *texttecture)
{
    static SDL_Rect healthBar;
    static SDL_Rect reloadTimer = {35, 0, 24, 0};

    SDL_RenderCopy(renderer, texttecture, &textrect[getPlayerweaponMag(player)/10], &textrect[3]);
    SDL_RenderCopy(renderer, texttecture, &textrect[getPlayerweaponMag(player)%10], &textrect[4]);
    SDL_RenderCopy(renderer, texttecture, &textrect[10], &textrect[2]);
    SDL_RenderCopy(renderer, texttecture, &textrect[2], &textrect[0]);
    SDL_RenderCopy(renderer, texttecture, &textrect[0], &textrect[1]);

    SDL_RenderCopy(renderer, texttecture, &textrect[getPlayerlives(player)], &textrect[7]);

    reloadTimer.h = round(((double)(90-getPlayerReloadprogress(player))/90)*22*(getPlayerReloadprogress(player)>0));
    reloadTimer.y = 24 - round(((double)(90-getPlayerReloadprogress(player))/90)*22*(getPlayerReloadprogress(player)>0));
    SDL_SetRenderDrawColor(renderer,255,255,255,200);
    SDL_RenderFillRect(renderer, &reloadTimer);
    
    healthBar.h = 14, healthBar.w = HEALTHBARLENGTH+4, healthBar.x = 98, healthBar.y = 5;
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderDrawRect(renderer, &healthBar);
    healthBar.h = 12, healthBar.w = HEALTHBARLENGTH+2, healthBar.x = 99, healthBar.y = 6;
    SDL_RenderDrawRect(renderer, &healthBar);
    healthBar.h = 10, healthBar.x = 100, healthBar.y = 7;
    healthBar.w = ((double)getPlayerHealth(player)/100)*HEALTHBARLENGTH;

    SDL_SetRenderDrawColor(renderer,255,0,0,175);
    SDL_RenderFillRect(renderer, &healthBar);
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