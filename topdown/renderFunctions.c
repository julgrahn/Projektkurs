#include "renderFunctions.h"

#define PUBLIC /* empty */
#define PRIVATE static

PUBLIC void renderMenu(SDL_Renderer* renderer, SDL_Texture* connectTexture, SDL_Texture* hostTexture, SDL_Texture* quitTexture, Button buttons[])
{
    SDL_Rect testRect = { 5, 10, 50, 100 };
    SDL_RenderClear(renderer);
    //SDL_RenderCopy(renderer, buttonTexture, NULL, &testRect);
    SDL_RenderCopy(renderer, connectTexture, NULL, getButtonRect(buttons[0]));
    SDL_RenderCopy(renderer, hostTexture, NULL, getButtonRect(buttons[1]));
    SDL_RenderCopy(renderer, quitTexture, NULL, getButtonRect(buttons[2]));

    SDL_RenderPresent(renderer);

}

PUBLIC void renderGame(SDL_Renderer* renderer, SDL_Texture* mTiles, SDL_Rect gTiles[], Bullet bullets[],
    SDL_Texture* bulletTexture, Player players[], SDL_Texture* playerText, SDL_Rect playerRect[], SDL_Point* playerRotationPoint)
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
    SDL_RenderPresent(renderer);
}