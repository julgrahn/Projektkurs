#include "menu.h"
#define PUBLIC

/*PUBLIC bool initMenu(SDL_Renderer** renderMenu)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return false;
    }

    SDL_Window* menu = SDL_CreateWindow("Main Menu", 20, 220, WINDOWWIDTH, WINDOWHEIGHT, 0);
    if (menu == NULL)
    {
        printf("error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    Uint32 renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    *renderMenu = SDL_CreateRenderer(menu, 1, renderFlags);
    if (renderMenu == NULL)
    {
        printf("error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(menu);
        SDL_Quit();
        return false;
    }
    return true;
}




PUBLIC void loadMenu(SDL_Renderer* renderMenu, SDL_Texture** buttons, SDL_Cursor** cursor)
{
    SDL_Surface* background = SDL_CreateRGBSurface(0, WINDOWWIDTH, WINDOWHEIGHT, 1, 0, 0, 0, 0);

    

    *buttons = SDL_CreateTextureFromSurface(renderMenu, connectButton);

}*/
