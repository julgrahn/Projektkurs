#include <stdio.h>
//  #include <SDL.h>
//  #include <SDL_image.h>
//  #include <SDL_timer.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <stdbool.h>
#include "player.h"
#include "world.h"

#define WINDOWWIDTH 704
#define WINDOWHEIGTH 704

bool init(SDL_Renderer **renderer);
void handleEvents(SDL_Event *event, int* up, int* down, int* right, int* left, bool* isPlaying);
void renderBackground(SDL_Renderer *gRenderer, SDL_Texture *mTiles, SDL_Rect gTiles[]);
void loadMedia(SDL_Renderer *renderer, SDL_Texture **tiles, SDL_Rect gridTiles[]);

int main(int argc, char* args[])
{
    SDL_Event event;
    SDL_Renderer* renderer = NULL;
    if (!init(&renderer)) return 1;

    SDL_Surface* testSurface = SDL_CreateRGBSurface(0, 20, 90, 1, 0, 0, 0, 0);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, testSurface);
    SDL_FreeSurface(testSurface);
    SDL_Rect testSquare;

    Player testPlayer = createPlayer(0, 0);

    bool isPlaying = true;
    int up = 0, down = 0, left = 0, right = 0;

    // Background
    SDL_Texture* tiles = NULL;
    SDL_Rect gridTiles[22];

    loadMedia(renderer, &tiles, gridTiles);

    while (isPlaying)
    {
        handleEvents(&event, &up, &down, &right, &left, &isPlaying);
      
        movePlayer(testPlayer, up, down, right, left);

        testSquare = getPlayerRect(testPlayer);
        SDL_RenderClear(renderer);

        renderBackground(renderer, tiles, gridTiles);
        SDL_RenderCopy(renderer, texture, NULL, &testSquare);
        SDL_RenderPresent(renderer);

        SDL_Delay(1000 / 60);
    }

    SDL_DestroyRenderer(renderer);
    //SDL_DestroyWindow(window); // beh�vs denna?
    SDL_Quit();

    return 0;
}
void loadMedia(SDL_Renderer *renderer, SDL_Texture **tiles, SDL_Rect gridTiles[]){
    SDL_Surface* gridTilesSurface = IMG_Load("resources/batch4.png");
    *tiles = SDL_CreateTextureFromSurface(renderer, gridTilesSurface);
    for (int i = 0; i < 22; i++) {
        gridTiles[i].x = i*getTileWidth();
        gridTiles[i].y = 0;
        gridTiles[i].w = getTileWidth();
        gridTiles[i].h = getTileHeight();
    }

}
void renderBackground(SDL_Renderer *gRenderer, SDL_Texture *mTiles, SDL_Rect gTiles[]){
    
    SDL_Rect possition;
    possition.y = 0;
    possition.x = 0;
    possition.h = getTileHeight();
    possition.w = getTileWidth();
    
    for (int i = 0; i<getTileColumns(); i++) {
        for (int j = 0; j<getTileRows(); j++) {
            possition.y = i*getTileHeight();
            possition.x = j*getTileWidth();
            SDL_RenderCopyEx(gRenderer, mTiles, &gTiles[getTileGrid(i,j)],&possition , 0, NULL, SDL_FLIP_NONE);
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
    SDL_Window* window = SDL_CreateWindow("top down extreme shooter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOWWIDTH, WINDOWHEIGTH, 0);
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