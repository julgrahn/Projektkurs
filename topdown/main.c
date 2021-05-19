#include <stdio.h>
#include "sdlinclude.h"
#include <stdbool.h>
#include <stdlib.h>
#include "player.h"
#include "world.h"
#include "bullet.h"
#include "server.h"
#include "networkgamestate.h"
#include "initFunctions.h"
#include "clientNetFunctions.h"
#include "gameFunctions.h"
#include "renderFunctions.h"

SDL_mutex* mutex;

void renderTestBullets(SDL_Renderer *renderer, Bullet bullets[][MAX_BULLETS], SDL_Texture *testText); // Synligare bullets för testing 

void handleEvents(SDL_Event* event, int* up, int* down, int* right, int* left, bool* isPlaying, SDL_Point *mouse, bool* shooting, bool *reload, bool *mute, int *tcpMessage, bool* scoreScreen);
void startPrompt(int* playerID, Server* server, bool* host);
void handleClientTCP(TCPsocket* tcpsock, SDLNet_SocketSet* set, Networkgamestate networkgamestate, Player players[], int playerID);

int main(int argc, char* args[])
{
    // Variables

    SDL_Event event;
    SDL_Renderer* renderer = NULL;
    UDPsocket sd;
    TCPsocket tcpsock = NULL;
    SDLNet_SocketSet set;
    IPaddress srvadd;
    UDPpacket* p;
    UDPpacket* p2;
    Server server = NULL;
    int playerID;
    SDL_Cursor* cursor = NULL;
    Player players[MAX_PLAYERS];
    SDL_Texture* playerText;
    SDL_Rect playerRect[4];
    SDL_Point mouse;
    mouse.x = 0, mouse.y = 0;
    Bullet bullets[MAX_PLAYERS][MAX_BULLETS];
    SDL_Texture* tiles = NULL;
    SDL_Rect gridTiles[900];   // Kommer innehålla alla 900 rutor från bakgrundsbilden, kan optmiseras.
    bool isPlaying = true, shooting = false, host = false, connected = false, reload = false, mute = false, scoreScreen = false;
    SDL_Texture* bulletTexture = NULL;
    SDL_Texture* gunFireTexture = NULL;
    SDL_Rect gunFireRect;
    Mix_Chunk* sound;
    Mix_Chunk* soundWall;
    Mix_Chunk* soundDeath;
    gunFireRect.w = 40;
    gunFireRect.h = 40;
    SDL_Texture* bloodTexture = NULL;
    SDL_Rect bloodRect;
    bloodRect.w = 64;
    bloodRect.h = 64;
    SDL_Rect bloodTiles[48];
    SDL_Texture* explosionTexture = NULL;
    SDL_Rect explosionRect;
    explosionRect.w = 40;
    explosionRect.h = 40;
    SDL_Texture *textTexture;
    SDL_Rect textRect[15];
    SDL_Rect healthBar;
    SDL_Rect reloadTimer;
    SDL_Rect aRoundStateRect[3];
    SDL_Texture* roundStateTexture;
    SDL_Rect aScoreRect[4];
    SDL_Texture* scoreTexture;
    SDL_Rect explosionTiles[121]; // Rutor från explosions.png
    int up = 0, down = 0, left = 0, right = 0;
    SDL_Point playerRotationPoint = { 20, 32 };
    SDL_Point muzzleRotationPoint = { 14, 16 };
    Networkgamestate networkgamestate = createNetworkgamestate();
    Button buttons[3];
    SDL_Texture* connectTextures[3];
    SDL_Texture* hostTextures[3];
    SDL_Texture* quitTextures[3];
    int tcpMessage = 0;


    // Init functions
    set = SDLNet_AllocSocketSet(1);
    mutex = SDL_CreateMutex();
    if (!initSDL(&renderer, &sound, &soundWall, &soundDeath)) return 1;
    initGameObjects(players, bullets);
    initGameHUD(renderer, textRect, &textTexture, &healthBar, &reloadTimer, aScoreRect, &scoreTexture, aRoundStateRect, &roundStateTexture);
    loadMenu(renderer, connectTextures, hostTextures, quitTextures);
    initClient(&sd, &p, &p2);
    loadMedia(renderer, gridTiles, &tiles, playerRect, &playerText, &cursor, &bulletTexture, 
            &gunFireTexture, &explosionTexture, &bloodTexture, 
            &sound, explosionTiles, bloodTiles, &soundWall, &soundDeath);


    // Synligare bullets för testing 
    SDL_Texture* bulletTEST;
    SDL_Surface* bulletSurface = IMG_Load("resources/bullet.png");
    bulletTEST = SDL_CreateTextureFromSurface(renderer, bulletSurface);
    SDL_FreeSurface(bulletSurface);
    
    // Menu loop
    buttons[0] = createButton((WINDOWWIDTH / 2) - BUTTON_HEIGHT, CONNECT_Y_POS);
    buttons[1] = createButton((WINDOWWIDTH / 2) - BUTTON_HEIGHT, HOST_Y_POS);
    buttons[2] = createButton((WINDOWWIDTH / 2) - BUTTON_HEIGHT, QUIT_Y_POS);
    while (isPlaying && !connected)
    {
        handleEvents(&event, &up, &down, &right, &left, &isPlaying, &mouse, &shooting, &reload, &mute, &tcpMessage, &scoreScreen);
        renderMenu(renderer, connectTextures, hostTextures, quitTextures, buttons, mouse.x, mouse.y, shooting);

        if (mouse.x >= (WINDOWWIDTH / 2) - BUTTON_HEIGHT && mouse.x <= (WINDOWWIDTH / 2) + BUTTON_HEIGHT)
        {
            // Connect button
            if (mouse.y > CONNECT_Y_POS && mouse.y < CONNECT_Y_POS + BUTTON_HEIGHT && shooting)
            {
                setButtonPressed(buttons[0], true);
                connectToServer(LOCAL_IP, &srvadd, &tcpsock, networkgamestate, &playerID, players, &sd, &connected);
                startUDPreceiveThread(&sd, &p2, bullets, players, &networkgamestate, playerID, &mutex);
                SDLNet_TCP_AddSocket(set, tcpsock);
            }

            // Host button
            if (mouse.y > HOST_Y_POS && mouse.y < HOST_Y_POS + BUTTON_HEIGHT && shooting)
            {
                server = createServer();
                startServer(server);
                host = true;
                setButtonPressed(buttons[1], true);
                connectToServer(LOCAL_IP, &srvadd, &tcpsock, networkgamestate, &playerID, players, &sd, &connected);
                startUDPreceiveThread(&sd, &p2, bullets, players, &networkgamestate, playerID, &mutex);
                SDLNet_TCP_AddSocket(set, tcpsock);
            }

            // Quit button
            if (mouse.y > QUIT_Y_POS && mouse.y < QUIT_Y_POS + BUTTON_HEIGHT && shooting)
            {
                setButtonPressed(buttons[2], true);
                isPlaying = false;
            }
        }
    }
    // Main loop
    while (isPlaying)
    {
        playerTick(players[playerID]);
        handleEvents(&event, &up, &down, &right, &left, &isPlaying, &mouse, &shooting, &reload, &mute, &tcpMessage, &scoreScreen);
        if (tcpMessage && host)
        {
            sendTCPtoServer(&tcpsock, tcpMessage);
            tcpMessage = 0;
        }
        if (mute)
        {
            Mix_Volume(-1, 0);
        }
        else
        {
            Mix_Volume(-1, 5);
        }
        if (isPlayerAlive(players[playerID]))
        {
            movePlayer(players[playerID], up, down, right, left, mouse.x, mouse.y, reload);
            if (shooting) fire(bullets[playerID], players[playerID]);
        }
        SDL_LockMutex(mutex);
        //Flytta på alla andra spelare
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            if (i != playerID) {
                moveOtherPlayers(players[i]);
            }
        }
        //printf("%d\n", getRoundState(networkgamestate)); // felsökningsprintf
        simulateBullets(bullets);
        setNetPlayer(networkgamestate, playerID, players[playerID]);
        setNetBullets(networkgamestate, playerID, bullets[playerID]);
        sendUDP(getNetPlayer(networkgamestate, playerID), &sd, &srvadd, &p, &p2);
        handleClientTCP(&tcpsock, &set, networkgamestate, players, playerID);

        renderGame(renderer, tiles, gridTiles, bullets, bulletTexture, players, playerText, 
                    playerRect, &playerRotationPoint, gunFireTexture, gunFireRect, 
                    explosionTexture, explosionRect,  &muzzleRotationPoint, bloodTexture, 
                    bloodRect, sound, explosionTiles, bloodTiles, soundWall, soundDeath);
        
        renderTestBullets(renderer, bullets, bulletTEST); // Synligare bullets för testing    

        renderHUD(renderer, players[playerID], textRect, textTexture, &healthBar, &reloadTimer);
        renderRoundState(renderer, aRoundStateRect, roundStateTexture, getRoundState(networkgamestate));
        if (scoreScreen) renderScoreScreen(renderer, aScoreRect, scoreTexture, textRect, textTexture, players);
        SDL_UnlockMutex(mutex);
        
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    //SDL_DestroyWindow(window); // beh�vs denna?
    SDL_Quit();
    return 0;
}

void handleEvents(SDL_Event* event, int* up, int* down, int* right, int* left, bool* isPlaying, SDL_Point *mouse, bool* shooting, bool *reload, bool *mute, int *tcpMessage, bool* scoreScreen)
{
    SDL_GetMouseState(&mouse->x, &mouse->y);
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
            case SDL_SCANCODE_P:
                *tcpMessage = 1;
                break;
            case SDL_SCANCODE_O:
                *tcpMessage = 2;
                break;
            case SDL_SCANCODE_TAB:
                *scoreScreen = true;
                break;
            case SDL_SCANCODE_R:
                *reload = true;
                break;
            case SDL_SCANCODE_M:
                if(*mute)
                {
                    *mute = false;
                }
                else
                {
                    *mute = true;
                }
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
            case SDL_SCANCODE_R:
                *reload = false;
                break;
            case SDL_SCANCODE_TAB:
                *scoreScreen = false;
                break;
            default:
                break;
            }
            break;


        case SDL_MOUSEBUTTONDOWN:

            switch (event->button.button)
            {
            case SDL_BUTTON_LEFT:
                *shooting = true;
            default:
                break;
            }
            break;
        case SDL_MOUSEBUTTONUP:

            switch (event->button.button)
            {
            case SDL_BUTTON_LEFT:
                *shooting = false;
            default:
                break;
            }
            break;
        }
    }
    return;
}


// Ska ersättas med menyn
void startPrompt(int* playerID, Server* server, bool* host)
{
    printf("Host(h) or client(c): ");
    char input;
    scanf(" %c", &input);
    if (input == 'h')
    {
        printf("hosted!\n");
        *host = true;
    }
}

void renderTestBullets(SDL_Renderer *renderer, Bullet bullets[][MAX_BULLETS], SDL_Texture *testText)
{
    for (int i = 0; i < MAX_PLAYERS; i++)
        for(int j = 0; j < MAX_BULLETS; j++)
            if(isBulletActive(bullets[i][j]))
                SDL_RenderCopy(renderer, testText, NULL, getBulletRect(bullets[i][j]));
}