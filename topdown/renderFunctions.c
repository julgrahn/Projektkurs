#include "renderFunctions.h"
#include <stdlib.h>

#define PUBLIC /* empty */
#define PRIVATE static

// PUBLIC void renderGame(SDL_Renderer* renderer, SDL_Texture* mTiles, SDL_Rect gTiles[], Bullet bullets[][MAX_BULLETS],
//     SDL_Texture* bulletTexture, Player players[], SDL_Texture* playerText, SDL_Rect playerRect[], SDL_Point* playerRotationPoint)
// int oldTileGrid[22][22] = { 0 }; // anv�nds f�r att animera v�ggtr�ffar. 

PUBLIC void renderGame(SDL_Renderer* renderer, SDL_Texture* mTiles, SDL_Rect gTiles[], Bullet bullets[][MAX_BULLETS],
    SDL_Texture* bulletTexture, Player players[], SDL_Texture* playerText, SDL_Rect playerRect[], SDL_Point* playerRotationPoint,
    SDL_Texture* gunFireTexture, SDL_Rect gunFireRect, SDL_Texture* explosionTexture, SDL_Rect explosionRect, 
    SDL_Point* muzzleRotationPoint, SDL_Texture* gunFireTexture2, SDL_Rect gunFireRect2, SDL_Point* bulletRotationPoint, Mix_Chunk* sound)
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
                SDL_RenderCopy(renderer, bulletTexture, NULL, getBulletRect(bullets[i][j]));
            }
            // SDL_RenderCopyEx(renderer, bulletTexture, NULL, getBulletRect(bullets[i]), getPlayerDirection(players[getBulletOwner(bullets[i])]), bulletRotationPoint, SDL_FLIP_NONE); //�ndra tillbaka till Rendercopy f�r att f� tillbaka r�tt skottlogik.
        }
    }
    // Render Players

    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (isPlayerAlive(players[i]))
        {
            SDL_RenderCopyEx(renderer, playerText, &playerRect[getPlayerFrame(players[i])], getPlayerRect(players[i]), getPlayerDirection(players[i]), playerRotationPoint, SDL_FLIP_NONE);
        }
    }
    // Render Gunfire
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        for(int j = 0; j < MAX_BULLETS; j++)
        {
            if(bulletHit(bullets[i][j]))
            {
                gunFireRect.x = getBulletX(bullets[i][j]) - 14;
                gunFireRect.y = getBulletY(bullets[i][j]) - 16;
                SDL_RenderCopy(renderer, gunFireTexture, NULL, &gunFireRect);
                // SDL_RenderCopyEx(renderer, gunFireTexture, NULL, &gunFireRect, NULL, NULL, SDL_FLIP_NONE);
            }
            if(bulletShot(bullets[i][j]))
            {
                gunFireRect.x = getBulletOriginX(bullets[i][j]) - 14;
                gunFireRect.y = getBulletOriginY(bullets[i][j]) - 16;
                // SDL_RenderCopy(renderer, gunFireTexture, NULL, &gunFireRect);
                SDL_RenderCopyEx(renderer, gunFireTexture, NULL, &gunFireRect, getPlayerDirection(players[i]), muzzleRotationPoint, SDL_FLIP_NONE);
                
                if(checkShot(bullets[i][j]))
                    Mix_PlayChannel(-1, sound, 0);
            }
        }
        // if (isPlayershooting(players[i]))
        // {
        //     gunFireRect.x = getPlayerX(players[i])+40;
        //     gunFireRect.y = getPlayerY(players[i])+26;
        //     // gunFireRect2.x = getPlayerX(players[i]) + 40;
        //     // gunFireRect2.y = getPlayerY(players[i]) + 26;

        //     SDL_RenderCopyEx(renderer, gunFireTexture, NULL, &gunFireRect, getPlayerDirection(players[i]), muzzleRotationPoint, SDL_FLIP_NONE);
        //     //SDL_RenderCopyEx(renderer, gunFireTexture2, NULL, &gunFireRect2, getPlayerDirection(players[i]), muzzleRotationPoint, SDL_FLIP_NONE); // gunfire2 �r ett f�rs�k att f� elden att skifta i storlek men fungerar ej.
        //     if(Mix_PlayChannel(-1, sound, 0) == -1)
        //     {
        //         printf("error: %s\n",Mix_GetError());
        //     }
        // }
    //     if (isPlayershooting(players[i]))
    //     {
    //         gunFireRect.x = getPlayerX(players[i])+40;
    //         gunFireRect.y = getPlayerY(players[i])+26;
    //         // gunFireRect2.x = getPlayerX(players[i]) + 40;
    //         // gunFireRect2.y = getPlayerY(players[i]) + 26;

    //         SDL_RenderCopyEx(renderer, gunFireTexture, NULL, &gunFireRect, getPlayerDirection(players[i]), muzzleRotationPoint, SDL_FLIP_NONE);
    //         //SDL_RenderCopyEx(renderer, gunFireTexture2, NULL, &gunFireRect2, getPlayerDirection(players[i]), muzzleRotationPoint, SDL_FLIP_NONE); // gunfire2 �r ett f�rs�k att f� elden att skifta i storlek men fungerar ej.
    //         if(Mix_PlayChannel(-1, sound, 0) == -1)
    //         {
    //             printf("error: %s\n",Mix_GetError());
    //         }
    //     }
}

    // Render Wallhits
    int tileRows = getTileRows();
    int tileColumns = getTileColumns();
    int k = 10;
    // for (int i = 0; i < tileRows; i++)
    // {
    //     for (int j = 0; j < tileColumns; j++)
    //     {
    //         if (getTileGridHits(i, j) != oldTileGrid[j][i])
    //         {
    //             explosionRect.x = j * 32;
    //             explosionRect.y = i * 32;
               
    //             SDL_RenderCopy(renderer, explosionTexture, NULL, &explosionRect); 
    //             oldTileGrid[j][i] = getTileGridHits(i, j);
    //         }
    //     }
    // }

    // Render Playerhits
    // for (int i = 0; i < MAX_PLAYERS; i++)
    // {
    //     if (checkIfPlayerdamaged(players[i]))
    //     {
    //         printf("damage\n"); 
    //         gunFireRect.x = getPlayerX(players[i]) + 16;
    //         gunFireRect.y = getPlayerY(players[i]) + 16;
    //         SDL_RenderCopy(renderer, gunFireTexture, NULL, &gunFireRect);
    //         resetDamagedPlayer(players[i]);
    //     }
    // }

    SDL_RenderPresent(renderer);
}

// PUBLIC void renderGame(SDL_Renderer* renderer, SDL_Texture* mTiles, SDL_Rect gTiles[], Bullet bullets[],
//     SDL_Texture* bulletTexture, Player players[], SDL_Texture* playerText, SDL_Rect playerRect[], SDL_Point* playerRotationPoint)
// {
//     SDL_RenderClear(renderer);
//     // Render Background
//     SDL_Rect position;
//     position.y = 0;
//     position.x = 0;
//     position.h = getTileHeight();
//     position.w = getTileWidth();

//     for (int i = 0; i < getTileColumns(); i++)
//     {
//         for (int j = 0; j < getTileRows(); j++)
//         {
//             position.y = i * getTileHeight();
//             position.x = j * getTileWidth();
//             SDL_RenderCopyEx(renderer, mTiles, &gTiles[getTileGrid(i, j)], &position, 0, NULL, SDL_FLIP_NONE);
//         }
//     }
//     // Render Bullets
//     for (int i = 0; i < MAX_BULLETS; i++)
//     {
//         if (isBulletActive(bullets[i]))
//         {
//             SDL_RenderCopy(renderer, bulletTexture, NULL, getBulletRect(bullets[i]));
//         }
//     }
//     // Render Players
//     for (int i = 0; i < MAX_PLAYERS; i++)
//     {
//         if (isPlayerAlive(players[i]))
//         {
//             SDL_RenderCopyEx(renderer, playerText, &playerRect[getPlayerFrame(players[i])], getPlayerRect(players[i]), getPlayerDirection(players[i]), playerRotationPoint, SDL_FLIP_NONE);
//         }
//     }
//     SDL_RenderPresent(renderer);
// }