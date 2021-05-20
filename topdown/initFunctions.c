#include "initFunctions.h"

#define PUBLIC /* empty */
#define PRIVATE static

PUBLIC bool initSDL(SDL_Renderer** renderer, Mix_Chunk** sound, Mix_Chunk** soundWall, Mix_Chunk** soundDeath)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) != 0)
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
    SDL_SetRenderDrawBlendMode(*renderer, SDL_BLENDMODE_BLEND);
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        SDL_Quit();
        return false;
    }
    Mix_AllocateChannels(100);
    return true;
}

PUBLIC void initGameObjects(Player players[], Bullet bullets[][MAX_BULLETS])
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        for (int j = 0; j < MAX_BULLETS; j++)
        {
            bullets[i][j] = createBullet();
        }
    }
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        players[i] = createPlayer(100, 100);
    }
}

PUBLIC void initClient(UDPsocket* sd, UDPpacket** p, UDPpacket** p2)
{
    if (SDLNet_Init() < 0)
    {
        fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    if (!(*sd = SDLNet_UDP_Open(0)))
    {
        fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    if (!((*p = SDLNet_AllocPacket(1024)) && (*p2 = SDLNet_AllocPacket(1024))))
    {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
}

PUBLIC void loadMedia(SDL_Renderer* renderer, SDL_Rect gTiles[], SDL_Texture** tiles, SDL_Rect playerRect[], 
                        SDL_Texture** pTexture, SDL_Cursor** cursor, SDL_Texture** bulletTexture, 
                        SDL_Texture** gunFireTexture, SDL_Texture** explosionTexture, 
                        SDL_Texture** bloodTexture, Mix_Chunk** sound,
                        SDL_Rect explosionTiles[], SDL_Rect bloodTiles[], Mix_Chunk** soundWall, Mix_Chunk** soundDeath, SDL_Texture** bloodTexture2, SDL_Rect bloodTiles2[])
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

    SDL_Surface* bulletSurface = IMG_Load("resources/expl_04_0014.png");
    *bulletTexture = SDL_CreateTextureFromSurface(renderer, bulletSurface);
    SDL_FreeSurface(bulletSurface);
     

    SDL_Surface* gunFireSurface = IMG_Load("resources/muzzle2_0007.png");
    *gunFireTexture = SDL_CreateTextureFromSurface(renderer, gunFireSurface);
    SDL_FreeSurface(gunFireSurface);

    SDL_Surface* explosionSurface = IMG_Load("resources/explosions.png");
    *explosionTexture = SDL_CreateTextureFromSurface(renderer, explosionSurface);
    SDL_FreeSurface(explosionSurface);
    for (int i = 0; i < 11; i++)
    {
        for (int j = 0; j < 11; j++)
        {
            explosionTiles[i * 11 + j].x = j * 93;
            explosionTiles[i * 11 + j].y = i * 93;
            explosionTiles[i * 11 + j].w = 93;
            explosionTiles[i * 11 + j].h = 93;
        }
    }

    // Blod
    SDL_Surface* bloodSurface = IMG_Load("resources/blood - right 1.png");
    *bloodTexture = SDL_CreateTextureFromSurface(renderer, bloodSurface);
    SDL_FreeSurface(bloodSurface);
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            bloodTiles[i * 4 + j].x = j * 512;
            bloodTiles[i * 4 + j].y = i * 512;
            bloodTiles[i * 4 + j].w = 512;
            bloodTiles[i * 4 + j].h = 512;
        }
    }

    SDL_Surface* bloodSurface2 = IMG_Load("resources/blood - left 1.png");
    *bloodTexture2 = SDL_CreateTextureFromSurface(renderer, bloodSurface2);
    SDL_FreeSurface(bloodSurface2);
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            bloodTiles2[i * 4 + j].x = j * 512;
            bloodTiles2[i * 4 + j].y = i * 512;
            bloodTiles2[i * 4 + j].w = 512;
            bloodTiles2[i * 4 + j].h = 512;
        }
    }
    


    // Gunfire Soundeffect
    *sound = Mix_LoadWAV("resources/gunsound2.wav");
    if (sound == NULL)
    {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
    }
    

    //Wall hit soundeffect
    *soundWall = Mix_LoadWAV("resources/thwack-10.wav");
    if (soundWall == NULL)
    {
        printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
    }
    // Volume
    

    *soundDeath = Mix_LoadWAV("resources/death.wav");
    if (soundDeath == NULL)
    {
        printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
    }
    Mix_Volume(-1, 5);
    
    
}

PUBLIC void initGameHUD(SDL_Renderer *renderer, SDL_Rect textRect[], SDL_Texture **textTexture, SDL_Rect *healthBar, SDL_Rect *reloadTimer, SDL_Rect aScorerect[], SDL_Texture **scoreTexture, SDL_Rect aRoundStateRect[], SDL_Texture **roundStateTexture)
{
    TTF_Init();
    TTF_Font *font;
    font=TTF_OpenFont("unispace_bd.ttf", 20);
    if(!font) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
    }
    else
    {
        // Ammo, hp och liv
        SDL_Color color = {255,255,255};
        SDL_Surface *textSurface;
        textSurface = TTF_RenderText_Solid(font,"0123456789|HP: ",color);
        *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface);
        healthBar->h = 10, healthBar->w = 150, healthBar->x = 100, healthBar->y = 7;
        reloadTimer->x = 35, reloadTimer->y = 2, reloadTimer->w = 24; 
        int width, height;
        TTF_SizeText(font, "0123456789|HP: ", &width, &height);
        for(int i = 0; i < 11; i++)
        {
            textRect[i].x = i * width/15;
            textRect[i].y = 0;
            textRect[i].w = width/15;
            textRect[i].h = height;
        }
        for(int i = 12; i < 15; i++)
        {
            textRect[i].x = i * width/15 + (width/15*4);
            textRect[i].y = 0;
            textRect[i].w = width/15;
            textRect[i].h = height;
        }
        textRect[11].x = 11 * width/15;
        textRect[11].y = 0;
        textRect[11].w = width/15*4;
        textRect[11].h = height;

        // Scorescreen

        aScorerect[0].x = 0;
        aScorerect[0].y = 0;
        aScorerect[0].w = width/15;
        aScorerect[0].h = height;

        textSurface = TTF_RenderText_Solid(font, "Player Kills", color);
        *scoreTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface);
        TTF_SizeText(font, "Player Kills", &width, &height);

        for (int i = 1; i < 3; i++)
        {
            aScorerect[i].x = 0;
            aScorerect[i].y = 0;
            aScorerect[i].w = width;
            aScorerect[i].h = height;
        }
        aScorerect[3].x = 0;
        aScorerect[3].y = 0;
        aScorerect[3].w = width;
        aScorerect[3].h = height;
        TTF_CloseFont(font);
        // Roundstate
        font = TTF_OpenFont("unispace_bd.ttf", 72);
        textSurface = TTF_RenderText_Solid(font, "WARMUP          PREPARE TO FIGHT", color);
        *roundStateTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface);
        TTF_SizeText(font, "WARMUP          PREPARE TO FIGHT", &width, &height);
        for (int i = 0; i < 3; i++)
        {
            aRoundStateRect[i].x = 0 + i*width/2;
            aRoundStateRect[i].y = 0;
            aRoundStateRect[i].w = width/2;
            aRoundStateRect[i].h = height;
        }

        
        TTF_CloseFont(font);
    }
}

PUBLIC void loadMenu(SDL_Renderer* renderer, SDL_Texture* connectTextures[], SDL_Texture* hostTextures[], SDL_Texture* quitTextures[])
{
    SDL_Surface* connectButton = IMG_Load("resources/ConnectGrey.png");
    SDL_Surface* connectButtonHover = IMG_Load("resources/ConnectWhite.png");
    SDL_Surface* connectButtonClick = IMG_Load("resources/ConnectGreen.png");

    SDL_Surface* hostButton = IMG_Load("resources/HostGrey.png");
    SDL_Surface* hostButtonHover = IMG_Load("resources/HostWhite.png");
    SDL_Surface* hostButtonClick = IMG_Load("resources/HostGreen.png");

    SDL_Surface* quitButton = IMG_Load("resources/QuitGrey.png");
    SDL_Surface* quitButtonHover = IMG_Load("resources/QuitWhite.png");
    SDL_Surface* quitButtonClick = IMG_Load("resources/QuitRed.png");

    connectTextures[0] = SDL_CreateTextureFromSurface(renderer, connectButton);
    connectTextures[1] = SDL_CreateTextureFromSurface(renderer, connectButtonHover);
    connectTextures[2] = SDL_CreateTextureFromSurface(renderer, connectButtonClick);

    hostTextures[0] = SDL_CreateTextureFromSurface(renderer, hostButton);
    hostTextures[1] = SDL_CreateTextureFromSurface(renderer, hostButtonHover);
    hostTextures[2] = SDL_CreateTextureFromSurface(renderer, hostButtonClick);

    quitTextures[0] = SDL_CreateTextureFromSurface(renderer, quitButton);
    quitTextures[1] = SDL_CreateTextureFromSurface(renderer, quitButtonHover);
    quitTextures[2] = SDL_CreateTextureFromSurface(renderer, quitButtonClick);

    SDL_FreeSurface(connectButton);
    SDL_FreeSurface(connectButtonHover);
    SDL_FreeSurface(connectButtonClick);

    SDL_FreeSurface(hostButton);
    SDL_FreeSurface(hostButtonHover);
    SDL_FreeSurface(hostButtonClick);

    SDL_FreeSurface(quitButton);
    SDL_FreeSurface(quitButtonHover);
    SDL_FreeSurface(quitButtonClick);

    /*if (connectTexture == NULL) printf("Finns ingen bild\n");
    else printf("texturen har laddats\n");*/

}