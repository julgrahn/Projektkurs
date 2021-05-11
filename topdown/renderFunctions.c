#include "renderFunctions.h"

#define PUBLIC /* empty */
#define PRIVATE static


/*PUBLIC void quitButton(SDL_Renderer* renderer, SDL_Texture* quitTextures[], Button buttons[], int mouseX, int mouseY, bool shooting)
{
    SDL_RenderClear(renderer);

    if (mouseX >= (WINDOWWIDTH / 2) - 64 && mouseX <= (WINDOWWIDTH / 2) + 64)
    {
        SDL_RenderCopy(renderer, quitTextures[0], NULL, getButtonRect(buttons[2]));

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
        SDL_RenderCopy(renderer, quitTextures[0], NULL, getButtonRect(buttons[2]));
    }

    SDL_RenderPresent(renderer);

}*/

PUBLIC void renderMenu(SDL_Renderer* renderer, SDL_Texture* connectTextures[], SDL_Texture* hostTextures[], SDL_Texture* quitTextures[], Button buttons[], int mouseX, int mouseY, bool shooting)
{
    
    SDL_RenderClear(renderer);
    
    // Connect button
    if (mouseX >= (WINDOWWIDTH / 2) - BUTTON_HEIGHT && mouseX <= (WINDOWWIDTH / 2) + BUTTON_HEIGHT)
    {
        SDL_RenderCopy(renderer, connectTextures[0], NULL, getButtonRect(buttons[0]));
        SDL_RenderCopy(renderer, hostTextures[0], NULL, getButtonRect(buttons[1]));
        SDL_RenderCopy(renderer, quitTextures[0], NULL, getButtonRect(buttons[2]));

        // Connect button

        if (mouseY > CONNECT_Y_POS && mouseY < CONNECT_Y_POS + BUTTON_HEIGHT)
        {
            SDL_RenderCopy(renderer, connectTextures[1], NULL, getButtonRect(buttons[0]));
        }

        if (mouseY > CONNECT_Y_POS && mouseY < CONNECT_Y_POS + BUTTON_HEIGHT && shooting)
        {

            SDL_RenderCopy(renderer, connectTextures[2], NULL, getButtonRect(buttons[0]));
        }

        // Host button

        if (mouseY > HOST_Y_POS && mouseY < HOST_Y_POS + BUTTON_HEIGHT)
        {
            SDL_RenderCopy(renderer, hostTextures[1], NULL, getButtonRect(buttons[1]));
        }

        if (mouseY > HOST_Y_POS && mouseY < HOST_Y_POS + BUTTON_HEIGHT && shooting)
        {

            SDL_RenderCopy(renderer, hostTextures[2], NULL, getButtonRect(buttons[1]));
        }

        // Quit button

        if (mouseY > QUIT_Y_POS && mouseY < QUIT_Y_POS + BUTTON_HEIGHT)
        {
            SDL_RenderCopy(renderer, quitTextures[1], NULL, getButtonRect(buttons[2]));
        }

        if (mouseY > QUIT_Y_POS && mouseY < QUIT_Y_POS + BUTTON_HEIGHT && shooting)
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