#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>
//#include <SDL2/SDL.h>
//#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_timer.h>
#include <stdbool.h>
#include "player.h"
#include "world.h"

#define WINDOWWIDTH 704
#define WINDOWHEIGHT 704

bool init(SDL_Renderer **renderer);
void handleEvents(SDL_Event *event, int* up, int* down, int* right, int* left, bool* isPlaying);
void renderBackground(SDL_Renderer *gRenderer, SDL_Texture *mTiles, SDL_Rect gTiles[]);
void loadMedia(SDL_Renderer *renderer, SDL_Rect gridTiles[], SDL_Texture **tiles);

int main(int argc, char* args[])
{
    SDL_Event event;
    SDL_Renderer* renderer = NULL;
    if (!init(&renderer)) return 1;

    SDL_Surface *playerSurface = IMG_Load("resources/playerRifle.PNG");
    SDL_Texture *playerText = SDL_CreateTextureFromSurface(renderer, playerSurface);
    SDL_FreeSurface(playerSurface);
    
    SDL_Rect playerRect[4];
    for(int n = 0; n < 4; n++)
    {
        playerRect[n].x = 0 + (n*64);
        playerRect[n].y = 0;
        playerRect[n].h = 64;
        playerRect[n].w = 64;
    }


    // Player
    Player testPlayer = createPlayer(0, 0);

    bool isPlaying = true;
    int up = 0, down = 0, left = 0, right = 0;

    // Background
    SDL_Texture* tiles = NULL;
    SDL_Rect gridTiles[22];

    loadMedia(renderer, gridTiles, &tiles);


    while (isPlaying)
    {
        handleEvents(&event, &up, &down, &right, &left, &isPlaying);
      
        movePlayer(testPlayer, up, down, right, left);

        SDL_RenderClear(renderer);
        //SDL_RenderCopy(renderer, texture, NULL, &testSquare);
        SDL_SetRenderDrawColor(renderer, 211, 211, 211, 255);
        SDL_RenderCopyEx(renderer, playerText, &playerRect[getPlayerFrame(testPlayer)], getPlayerRect(testPlayer), 0, NULL, SDL_FLIP_NONE);

        renderBackground(renderer, tiles, gridTiles);
        SDL_RenderCopy(renderer, texture, NULL, &testSquare);
        SDL_RenderPresent(renderer);
        

        SDL_Delay(1000 / 60);
    }

    //Game renderer

    SDL_DestroyRenderer(renderer);
    //SDL_DestroyWindow(window); // beh�vs denna?
    SDL_Quit();

    return 0;
}


void loadMedia(SDL_Renderer *renderer, SDL_Rect gTiles[], SDL_Texture **tiles)
{
    SDL_Surface* gTilesSurface = IMG_Load("resources/tilemap.png");
    *tiles = SDL_CreateTextureFromSurface(renderer, gTilesSurface);

    for (int i = 0; i < 22; i++)
    {
        gTiles[i].x = i * getTileWidth();
        gTiles[i].y = 0;
        gTiles[i].w = getTileWidth();
        gTiles[i].h = getTileHeight();
    }
}

void renderBackground(SDL_Renderer *gRenderer, SDL_Texture *mTiles, SDL_Rect gTiles[])
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

void handleEvents(SDL_Event *event, int* up, int* down, int* right, int* left, bool* isPlaying)
{
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