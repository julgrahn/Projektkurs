#include <stdio.h>
//  #include <SDL.h>
//  #include <SDL_image.h>
//  #include <SDL_timer.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#define WINDOWWIDTH 1280
#define WINDOWHEIGTH 720

int main(int argc, char* args[])
{

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Window *window = SDL_CreateWindow("top down extreme shooter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOWWIDTH, WINDOWHEIGTH, 0);
    if (window == NULL)
    {
        printf("error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    Uint32 renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, renderFlags);
    if (renderer == NULL)
    {
        printf("error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Surface *testSurface = SDL_CreateRGBSurface(0, 20, 90, 1, 0, 0, 0, 0);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, testSurface);
    SDL_FreeSurface(testSurface);
    SDL_Rect testSquare;
    testSquare.x = 240;
    testSquare.y = 360;
    testSquare.w = 20;
    testSquare.h = 90;

    bool isPlaying = true;
    int up = 0, down = 0, left = 0, right = 0;

    while (isPlaying)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    isPlaying = false;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.scancode)
                    {
                        case SDL_SCANCODE_W:
                        case SDL_SCANCODE_UP:
                            up = 1;
                            break;
                        case SDL_SCANCODE_A:
                        case SDL_SCANCODE_LEFT:
                            left = 1;
                            break;
                        case SDL_SCANCODE_S:
                        case SDL_SCANCODE_DOWN:
                            down = 1;
                            break;
                        case SDL_SCANCODE_D:
                        case SDL_SCANCODE_RIGHT:
                            right = 1;
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (event.key.keysym.scancode)
                    {
                        case SDL_SCANCODE_W:
                        case SDL_SCANCODE_UP:
                            up = 0;
                            break;
                        case SDL_SCANCODE_A:
                        case SDL_SCANCODE_LEFT:
                            left = 0;
                            break;
                        case SDL_SCANCODE_S:
                        case SDL_SCANCODE_DOWN:
                            down = 0;
                            break;
                        case SDL_SCANCODE_D:
                        case SDL_SCANCODE_RIGHT:
                            right = 0;
                            break;
                        default:
                            break;
                    }
                    break;
            }
        }

        // x_vel = y_vel = 0;
        if (up && !down) testSquare.y -= 2;
        if (down && !up) testSquare.y += 2;
        if (left && !right) testSquare.x -= 2;
        if (right && !left) testSquare.x += 2;

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, &testSquare);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000 / 60);
    }
  
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}