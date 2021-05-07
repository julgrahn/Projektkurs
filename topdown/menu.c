#include "menu.h"
#define PUBLIC

PUBLIC bool initMenu(SDL_Renderer** renderMenu)
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

    SDL_Surface* connectButton = IMG_Load("reources/ConnectGrey.png");
    SDL_Surface* connectButtonHover = IMG_Load("reources/ConnectWhite.png");
    SDL_Surface* connectButtonClick = IMG_Load("reources/ConnectGreen.png");

    SDL_Surface* hostButton = IMG_Load("resources/HostGrey.png");
    SDL_Surface* hostButtonHover = IMG_Load("resources/HostWhite.png");
    SDL_Surface* hostButtonClick = IMG_Load("resources/HostGreen.png");

    SDL_Surface* quitButton = IMG_Load("resources/QuitGrey.png");
    SDL_Surface* quitButtonHover = IMG_Load("resources/QuitWhite.png");
    SDL_Surface* quitButtonClick = IMG_Load("resources/QuitRed.png");

    *buttons = SDL_CreateTextureFromSurface(renderMenu, connectButton);



}
