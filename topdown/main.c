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
#include "bullet.h"

<<<<<<< Updated upstream
#define WINDOWWIDTH 704
#define WINDOWHEIGHT 704

bool init(SDL_Renderer** renderer);
void handleEvents(SDL_Event* event, int* up, int* down, int* right, int* left, bool* isPlaying);
void renderBackground(SDL_Renderer* gRenderer, SDL_Texture* mTiles, SDL_Rect gTiles[]);
void loadMedia(SDL_Renderer* renderer, SDL_Rect gridTiles[], SDL_Texture** tiles);
=======


bool init(SDL_Renderer** renderer);
void handleEvents(SDL_Event* event, int* up, int* down, int* right, int* left, bool* isPlaying, int* mouseX, int* mouseY, bool* shooting);
void renderBackground(SDL_Renderer* gRenderer, SDL_Texture* mTiles, SDL_Rect gTiles[]);
void loadMedia(SDL_Renderer* renderer, SDL_Rect gTiles[], SDL_Texture** tiles, SDL_Rect playerRect[], SDL_Texture** pTexture, SDL_Cursor** cursor);
>>>>>>> Stashed changes

int main(int argc, char* args[])
{
    SDL_Event event;
    SDL_Renderer* renderer = NULL;
    if (!init(&renderer)) return 1;

    SDL_Surface* testSurface = SDL_CreateRGBSurface(0, 20, 90, 1, 0, 0, 0, 0);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, testSurface);
    SDL_FreeSurface(testSurface);
    SDL_Rect testSquare;

    // Player
<<<<<<< Updated upstream
    Player testPlayer = createPlayer(0, 0);
=======
    Player player1 = createPlayer(0, 0);
    SDL_Texture* playerText;
    SDL_Rect playerRect[4];
    int mouseX = 0, mouseY = 0;
>>>>>>> Stashed changes

    // Bullet
    Bullet bullets[MAX_BULLETS];
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        bullets[i] = createBullet();
    }
    SDL_Surface* bulletSurface = IMG_Load("resources/bullet.png");
    SDL_Texture* bulletTexture = SDL_CreateTextureFromSurface(renderer, bulletSurface);

    bool isPlaying = true, shooting = false;
    int up = 0, down = 0, left = 0, right = 0;

    // Background
    SDL_Texture* tiles = NULL;
    SDL_Rect gridTiles[900];   // Kommer innehålla alla 900 rutor från bakgrundsbilden, kan optmiseras.

    loadMedia(renderer, gridTiles, &tiles);

<<<<<<< Updated upstream

    while (isPlaying)
    {
        handleEvents(&event, &up, &down, &right, &left, &isPlaying);

        movePlayer(testPlayer, up, down, right, left);

        testSquare = getPlayerRect(testPlayer);
=======
    SDL_Point playerRotationPoint = { 20, 32 };

    while (isPlaying)
    {
        handleEvents(&event, &up, &down, &right, &left, &isPlaying, &mouseX, &mouseY, &shooting);

        movePlayer(player1, up, down, right, left, mouseX, mouseY);
        if (shooting)
        {
            for (int i = 0; i < MAX_BULLETS; i++)
            {
                if (!isBulletActive(bullets[i]))
                {
                    spawnBullet(bullets[i], getPlayerX(player1), getPlayerY(player1), getPlayerDirection(player1));
                    break;
                }
            }
        }
        for (int i = 0; i < MAX_BULLETS; i++)
        {
            if (isBulletActive(bullets[i]))
            {
                moveBullet(bullets[i]);
            }
        }
        
>>>>>>> Stashed changes
        SDL_RenderClear(renderer);

        renderBackground(renderer, tiles, gridTiles);
<<<<<<< Updated upstream
        SDL_RenderCopy(renderer, texture, NULL, &testSquare);
        SDL_RenderPresent(renderer);


=======
        SDL_RenderCopyEx(renderer, playerText, &playerRect[getPlayerFrame(player1)], getPlayerRect(player1), getPlayerDirection(player1), &playerRotationPoint, SDL_FLIP_NONE);
        for (int i = 0; i < MAX_BULLETS; i++)
        {
            if (isBulletActive(bullets[i]))
            {
                SDL_RenderCopy(renderer, bulletTexture, NULL, bullets[i]);
            }
            
        }
        
        SDL_RenderPresent(renderer);

>>>>>>> Stashed changes
        SDL_Delay(1000 / 60);
        
    }

    //Game renderer

    SDL_DestroyRenderer(renderer);
    //SDL_DestroyWindow(window); // beh�vs denna?
    SDL_Quit();

    return 0;
}

<<<<<<< Updated upstream

void loadMedia(SDL_Renderer* renderer, SDL_Rect gTiles[], SDL_Texture** tiles)
=======
void loadMedia(SDL_Renderer* renderer, SDL_Rect gTiles[], SDL_Texture** tiles, SDL_Rect playerRect[], SDL_Texture** pTexture, SDL_Cursor** cursor)
>>>>>>> Stashed changes
{
    SDL_Surface* gTilesSurface = IMG_Load("resources/tilemap.png");
    *tiles = SDL_CreateTextureFromSurface(renderer, gTilesSurface);

    for (int i = 0; i < 30; i++)
    {
        for (int j = 0; j < 30; j++)
        {
            gTiles[i * 30 + j].x = j * getTileWidth();
            gTiles[i * 30 + j].y = i * getTileHeight();
            gTiles[i * 30 + j].w = getTileWidth();
            gTiles[i * 30 + j].h = getTileHeight();
        }
<<<<<<< Updated upstream
        
    }
 
=======

    }
    SDL_Surface* playerSurface = IMG_Load("resources/playerRifle.png");
    *pTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
    SDL_FreeSurface(playerSurface);
    for (int n = 0; n < 4; n++)
    {
        playerRect[n].x = 0 + (n * 64);
        playerRect[n].y = 0;
        playerRect[n].h = 64;
        playerRect[n].w = 64;
    }
    SDL_Surface* cursorSurface = IMG_Load("resources/crosshair161.png");
    *cursor = SDL_CreateColorCursor(cursorSurface, 36, 36);
    SDL_FreeSurface(cursorSurface);
    SDL_SetCursor(*cursor);
>>>>>>> Stashed changes
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



bool init(SDL_Renderer** renderer)
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

<<<<<<< Updated upstream
void handleEvents(SDL_Event* event, int* up, int* down, int* right, int* left, bool* isPlaying)
=======
void handleEvents(SDL_Event* event, int* up, int* down, int* right, int* left, bool* isPlaying, int* mouseX, int* mouseY, bool* shooting)
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
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
=======
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
>>>>>>> Stashed changes
            default:
                break;
            }
            break;
<<<<<<< Updated upstream
=======
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
        case SDL_MOUSEBUTTONDOWN: //KP
           
            *shooting = true;           
             break;           
            
        case SDL_MOUSEBUTTONUP: //KP
                *shooting = false;
                break;

>>>>>>> Stashed changes
        }
        
    }
    return;
}