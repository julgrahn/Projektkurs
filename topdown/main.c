#include <stdio.h>
// #include <SDL.h>
// #include <SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

int main(int argc, char* args[]){
    
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1; 
    }
    SDL_Window *window = SDL_CreateWindow("top down extreme shooter", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN); 
    if(window == NULL)
    {
        printf("error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    Uint32 renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, renderFlags);
    if(renderer == NULL)
    {
        printf("error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    while(1)
    {
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
        //SDL_RenderCopy(renderer, );

        SDL_Delay(1000/60);
    }
    return 0;
}