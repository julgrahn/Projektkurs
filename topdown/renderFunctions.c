#include "renderFunctions.h"
#include <stdlib.h>

#define PUBLIC /* empty */
#define PRIVATE static

PUBLIC void renderGame(SDL_Renderer* renderer, SDL_Texture* mTiles, SDL_Rect gTiles[], Bullet bullets[][MAX_BULLETS],
    SDL_Texture* bulletTexture, Player players[], SDL_Texture* playerText, SDL_Rect playerRect[], SDL_Point* playerRotationPoint,
    SDL_Texture* gunFireTexture, SDL_Rect gunFireRect, SDL_Texture* explosionTexture, SDL_Rect explosionRect, 
    SDL_Point* muzzleRotationPoint, SDL_Texture* bloodTexture, SDL_Rect bloodRect, Mix_Chunk* sound,
    SDL_Rect explosionTiles[], SDL_Rect bloodTiles[])
{
    SDL_RenderClear(renderer);
    // Render Background
    SDL_Rect position;
    position.y = 0;
    position.x = 0;
    position.h = getTileHeight();
    position.w = getTileWidth();

    for (int i = 0; i < getTileColumns(); i++)
    {
        for (int j = 0; j < getTileRows(); j++)
        {
            position.y = i * getTileHeight();
            position.x = j * getTileWidth();
            SDL_RenderCopyEx(renderer, mTiles, &gTiles[getTileGrid(i, j)], &position, 0, NULL, SDL_FLIP_NONE);
        }
    }
    // Render Bullets
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        for(int j = 0; j < MAX_BULLETS; j++)
        {
            if (isBulletActive(bullets[i][j]))
            {
                SDL_RenderCopyEx(renderer, bulletTexture, &explosionTiles[12], getBulletRect(bullets[i][j]), 0 , NULL, SDL_FLIP_NONE);
            } 
        }
    }
    // Render Players

    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (isPlayerAlive(players[i]))
        {
            SDL_RenderCopyEx(renderer, playerText, &playerRect[getPlayerFrame(players[i])], getPlayerRect(players[i]), getPlayerDirection(players[i]), playerRotationPoint, SDL_FLIP_NONE);
            printf("%d\n", getPlayerDirection(players[i])); 
        }
    }
    // Render Gunfire
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        for (int j = 0; j < MAX_BULLETS; j++)
        {
            if (bulletHit(bullets[i][j]))
            {
                gunFireRect.x = getBulletX(bullets[i][j]) - 14; 
                gunFireRect.y = getBulletY(bullets[i][j]) - 16;
                if (getWallCollisionBullet(getBulletX(bullets[i][j]), getBulletY(bullets[i][j]), 4, 4))
                {
                    if (getBulletHitValue(bullets[i][j]) > 12)
                    {
                        SDL_RenderCopyEx(renderer, explosionTexture, &explosionTiles[45], &gunFireRect, 0, NULL, SDL_FLIP_NONE);
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
                    if (getBulletHitValue(bullets[i][j]) > 12)
                    {
                        SDL_RenderCopyEx(renderer, bloodTexture, &bloodTiles[9], &gunFireRect, 0, NULL, SDL_FLIP_NONE);
                    }
                    else if (getBulletHitValue(bullets[i][j]) > 9)
                    {
                        SDL_RenderCopyEx(renderer, bloodTexture, &bloodTiles[10], &gunFireRect, 0, NULL, SDL_FLIP_NONE);
                    }
                    else if (getBulletHitValue(bullets[i][j]) > 6)
                    {
                        SDL_RenderCopyEx(renderer, bloodTexture, &bloodTiles[11], &gunFireRect, 0, NULL, SDL_FLIP_NONE);
                    }
                    else if (getBulletHitValue(bullets[i][j]) > 3)
                    {
                        SDL_RenderCopyEx(renderer, bloodTexture, &bloodTiles[12], &gunFireRect, 0, NULL, SDL_FLIP_NONE);
                    }
                }
                
            }
            if (bulletShot(bullets[i][j]))
            {
                gunFireRect.x = getBulletOriginX(bullets[i][j]) - 14;
                gunFireRect.y = getBulletOriginY(bullets[i][j]) - 16;
                // SDL_RenderCopy(renderer, gunFireTexture, NULL, &gunFireRect);
                SDL_RenderCopyEx(renderer, gunFireTexture, NULL, &gunFireRect, getPlayerDirection(players[i]), muzzleRotationPoint, SDL_FLIP_NONE);

                // if(checkShot(bullets[i][j]))
                //     Mix_PlayChannel(-1, sound, 0);
            }
        }
    }
    SDL_RenderPresent(renderer);
}

