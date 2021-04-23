#include <stdio.h>
#include "sdlinclude.h"
#include <stdbool.h>
#include "player.h"
#include "world.h"

bool init(SDL_Renderer **renderer);
void handleEvents(SDL_Event *event, int* up, int *down, int *right, int *left, bool *isPlaying, int *mouseX, int *mouseY);
void renderBackground(SDL_Renderer *gRenderer, SDL_Texture *mTiles, SDL_Rect gTiles[]);
void loadMedia(SDL_Renderer *renderer, SDL_Rect gTiles[], SDL_Texture **tiles, SDL_Rect playerRect[], SDL_Texture **pTexture, SDL_Cursor **cursor);

int main(int argc, char *args[])
{
    SDL_Event event;
    SDL_Renderer *renderer = NULL;
    if (!init(&renderer)) return 1;

    SDL_Cursor *cursor = NULL;

    // Player
    Player player1 = createPlayer(0, 0);
    SDL_Texture *playerText;
    SDL_Rect playerRect[4];
    int mouseX = 0, mouseY = 0;

    bool isPlaying = true;
    int up = 0, down = 0, left = 0, right = 0;

    // Background
    SDL_Texture *tiles = NULL;
    SDL_Rect gridTiles[900];   // Kommer innehålla alla 900 rutor från bakgrundsbilden, kan optmiseras.

    loadMedia(renderer, gridTiles, &tiles, playerRect, &playerText, &cursor);

    SDL_Point playerRotationPoint = {20, 32};

    while (isPlaying)
    {
        handleEvents(&event, &up, &down, &right, &left, &isPlaying, &mouseX, &mouseY);

        movePlayer(player1, up, down, right, left, mouseX, mouseY);

        SDL_RenderClear(renderer);

        //Game renderer
        renderBackground(renderer, tiles, gridTiles);
        SDL_RenderCopyEx(renderer, playerText, &playerRect[getPlayerFrame(player1)], getPlayerRect(player1), getPlayerDirection(player1), &playerRotationPoint, SDL_FLIP_NONE);
        SDL_RenderPresent(renderer);
        
        SDL_Delay(1000 / 60);
    }


    SDL_DestroyRenderer(renderer);
    //SDL_DestroyWindow(window); // beh�vs denna?
    SDL_Quit();

    //Network
    if (SDL_Init(0) == -1)
    {
        printf("SDL_Init: %s\n", SDL_GetError());
        exit(1);
    }

    if (SDLNet_Init() == -1)
    {
        printf("SDLNet_init: %s", SDLNet_GetError());
        exit(2);
    }

    return 0;
}


void loadMedia(SDL_Renderer *renderer, SDL_Rect gTiles[], SDL_Texture **tiles, SDL_Rect playerRect[], SDL_Texture **pTexture, SDL_Cursor **cursor)
{
    SDL_Surface* gTilesSurface = IMG_Load("resources/tilemap.png");
    *tiles = SDL_CreateTextureFromSurface(renderer, gTilesSurface);
    SDL_FreeSurface(gTilesSurface);
    for (int i = 0; i < 30; i++)
    {
        for (int j = 0; j < 30; j++)
        {
            gTiles[i * 30 + j].x = j * getTileWidth();
            gTiles[i * 30 + j].y = i * getTileHeight();
            gTiles[i * 30 + j].w = getTileWidth();
            gTiles[i * 30 + j].h = getTileHeight();
        }

    }
    SDL_Surface *playerSurface = IMG_Load("resources/playerRifle.png");
    *pTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
    SDL_FreeSurface(playerSurface);
    for(int n = 0; n < 4; n++)
    {
        playerRect[n].x = 0 + (n*64);
        playerRect[n].y = 0;
        playerRect[n].h = 64;
        playerRect[n].w = 64;
    }
    SDL_Surface *cursorSurface = IMG_Load("resources/crosshair161.png");
    *cursor = SDL_CreateColorCursor(cursorSurface, 36, 36);
    SDL_FreeSurface(cursorSurface);
    SDL_SetCursor(*cursor);
}

void renderBackground(SDL_Renderer* gRenderer, SDL_Texture* mTiles, SDL_Rect gTiles[])
{
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
            SDL_RenderCopyEx(gRenderer, mTiles, &gTiles[getTileGrid(i, j)], &position, 0, NULL, SDL_FLIP_NONE);
        }
    }
}



bool init(SDL_Renderer **renderer)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return false;
    }
    SDL_Window* window = SDL_CreateWindow("top down extreme shooter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOWWIDTH, WINDOWHEIGHT, 0);
    if (window == NULL)
    {
        printf("error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }
    Uint32 renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;

    *renderer = SDL_CreateRenderer(window, -1, renderFlags);
    if (renderer == NULL)
    {
        printf("error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }
    return true;
}

void handleEvents(SDL_Event *event, int* up, int* down, int* right, int* left, bool* isPlaying, int *mouseX, int *mouseY)
{
    SDL_GetMouseState(mouseX, mouseY);
    while (SDL_PollEvent(event))
    {
        switch (event->type)
        {
            case SDL_QUIT:
                *isPlaying = false;
                break;
            case SDL_KEYDOWN:
                switch (event->key.keysym.scancode)
                {
                    case SDL_SCANCODE_W:
                    case SDL_SCANCODE_UP:
                        *up = 1;
                        break;
                    case SDL_SCANCODE_A:
                    case SDL_SCANCODE_LEFT:
                        *left = 1;
                        break;
                    case SDL_SCANCODE_S:
                    case SDL_SCANCODE_DOWN:
                        *down = 1;
                        break;
                    case SDL_SCANCODE_D:
                    case SDL_SCANCODE_RIGHT:
                        *right = 1;
                        break;
                    default:
                        break;
                }
                break;
            case SDL_KEYUP:
                switch (event->key.keysym.scancode)
                {
                    case SDL_SCANCODE_W:
                    case SDL_SCANCODE_UP:
                        *up = 0;
                        break;
                    case SDL_SCANCODE_A:
                    case SDL_SCANCODE_LEFT:
                        *left = 0;
                        break;
                    case SDL_SCANCODE_S:
                    case SDL_SCANCODE_DOWN:
                        *down = 0;
                        break;
                    case SDL_SCANCODE_D:
                    case SDL_SCANCODE_RIGHT:
                        *right = 0;
                        break;
                    default:
                        break;
                }
                break;
        }
    }
    return;
}