#include "renderFunctions.h"
#include <stdlib.h>

#define PUBLIC /* empty */
#define PRIVATE static

PUBLIC void renderGame(SDL_Renderer* renderer, SDL_Texture* mTiles, SDL_Rect gTiles[], Bullet bullets[],
    SDL_Texture* bulletTexture, Player players[], SDL_Texture* playerText, SDL_Rect playerRect[], SDL_Point* playerRotationPoint,
     SDL_Texture** gunFireTexture, SDL_Rect gunFireRect)
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
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (isBulletActive(bullets[i]))
        {
            SDL_RenderCopy(renderer, bulletTexture, NULL, getBulletRect(bullets[i]));
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
        if (isPlayershooting(players[i]))
        {
            gunFireRect.x = getPlayerX(players[i]) + 32;
            gunFireRect.y = getPlayerY(players[i]) + 32;

            SDL_RenderCopyEx(renderer, gunFireTexture, NULL, &gunFireRect, getPlayerDirection(players[i]), playerRotationPoint, SDL_FLIP_NONE);
        }
    }

    // Render Wallhits
    int oldTileGrid[22][22] = { 0 };
    int tileRows = getTileRows();
    int tileColumns = getTileColumns();
    for (int i = 0; i < tileRows; i++)
    {
        for (int j = 0; j < tileColumns; j++)
        {
            if (getTileGridHits(i, j) != oldTileGrid[j][i])
            {
                gunFireRect.x = j * 32;
                gunFireRect.y = i * 32;
                SDL_RenderCopy(renderer, gunFireTexture, NULL, &gunFireRect);
                oldTileGrid[j][i] = getTileGridHits(i, j);
            }
        }
    }

    // Render Playerhits
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (checkIfPlayerdamaged(players[i]))
        {
            printf("damage\n");
            SDL_RenderCopy(renderer, gunFireTexture, NULL, &gunFireRect);
            // resetDamagedPlayer(players[i]);
        }
    }

    SDL_RenderPresent(renderer);
}