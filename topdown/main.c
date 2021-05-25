#pragma warning(disable : 4996)

#include <stdio.h>
#include "sdlinclude.h"
#include <stdbool.h>
#include <string.h>
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
void handleClientTCP(TCPsocket* tcpsock, SDLNet_SocketSet* set, Networkgamestate networkgamestate, Player players[], int playerID);
void connectPrompt(char* ip);

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
    SDL_Point mouse = {0, 0};
    Bullet bullets[MAX_PLAYERS][MAX_BULLETS];
    SDL_Texture* tiles = NULL;
    SDL_Rect gridTiles[900];   // Kommer innehålla alla 900 rutor från bakgrundsbilden, kan optmiseras.
    bool isPlaying = true, shooting = false, host = false, connected = false, reload = false, mute = false, scoreScreen = false;
    SDL_Texture* bulletTexture = NULL;
    SDL_Texture* gunFireTexture = NULL;
    Mix_Chunk* sound;
    Mix_Chunk* soundWall;
    Mix_Chunk* soundDeath;
    Mix_Chunk* prepareToFight;
    SDL_Texture* bloodTexture = NULL;
    SDL_Rect bloodTiles[48];
    SDL_Rect bloodTiles2[48];
    SDL_Texture* explosionTexture = NULL;
    SDL_Texture *textTexture;
    SDL_Rect textRect[15];
    SDL_Rect aRoundStateRect[4];
    SDL_Texture* roundStateTexture;
    SDL_Rect aScoreRect[4];
    SDL_Texture* scoreTexture;
    SDL_Rect explosionTiles[121]; // Rutor från explosions.png
    int up = 0, down = 0, left = 0, right = 0;
    Networkgamestate networkgamestate = createNetworkgamestate();
    Button buttons[3];
    char hostIP[20];
    int tcpMessage = 0;
    int winner = 0;
    bool newRoundFlag = false;
    SDL_Texture* connectTextures[3];
    SDL_Texture* hostTextures[3];
    SDL_Texture* quitTextures[3];

    // Init functions
    set = SDLNet_AllocSocketSet(1);
    mutex = SDL_CreateMutex();
    if (!initSDL(&renderer, &sound, &soundWall, &soundDeath)) return 1;
    initGameObjects(players, bullets);
    initGameHUD(renderer, textRect, &textTexture, aScoreRect, &scoreTexture, aRoundStateRect, &roundStateTexture);
    loadMenu(renderer, connectTextures, hostTextures, quitTextures);
    initClient(&sd, &p, &p2);
    loadMedia(renderer, gridTiles, &tiles, playerRect, &playerText, &cursor, &bulletTexture, 
            &gunFireTexture, &explosionTexture, &bloodTexture, 
            &sound, explosionTiles, bloodTiles, &soundWall, &soundDeath, &bloodTexture2, bloodTiles2, &prepareToFight);
            

    // För ny runda återställ kartan
    initTileGridReset();

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
                connectPrompt(hostIP);
                printf("%s", hostIP);
                connectToServer(hostIP, &srvadd, &tcpsock, networkgamestate, &playerID, players, &sd, &connected);
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
    //Spel
    // Main loop
    while (isPlaying)
    {
        //Om ny runda
        if (getRoundState(networkgamestate) == 1)
        {
            if (newRoundFlag)
            {
                newRound(prepareToFight);
                newRoundFlag = false;
            }
        }
        else
        {
            newRoundFlag = true;
        }
        //Ljud av/på
        Mix_Volume(-1, 5*!mute);
        
        playerTick(players[playerID]);

        if (getRoundState(networkgamestate) == 3)
        {
            for (int i = 0; i < MAX_PLAYERS; i++)
            {
                if (isPlayerAlive(players[i]))
                {
                    winner = i;
                    break;
                }
            }
        }
        handleEvents(&event, &up, &down, &right, &left, &isPlaying, &mouse, &shooting, &reload, &mute, &tcpMessage, &scoreScreen);
        if (tcpMessage && host)
        {
            sendTCPtoServer(&tcpsock, tcpMessage);
            tcpMessage = 0;
        }
        SDL_LockMutex(mutex);
        if (isPlayerAlive(players[playerID]))
        {
            movePlayer(players[playerID], up, down, right, left, mouse.x, mouse.y, reload);
            if (shooting) fire(bullets[playerID], players[playerID]);
        }
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

        SDL_RenderClear(renderer);

        renderGame(renderer, tiles, gridTiles, bullets, bulletTexture, players, playerText, playerRect, 
                    gunFireTexture, explosionTexture, bloodTexture, sound, explosionTiles, bloodTiles, soundWall, soundDeath);
        
        renderTestBullets(renderer, bullets, bulletTEST); // Synligare bullets för testing    

        renderHUD(renderer, players[playerID], textRect, textTexture);
          
        renderRoundState(renderer, aRoundStateRect, roundStateTexture, getRoundState(networkgamestate), winner, textRect, textTexture);
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


void connectPrompt(char* hostIP)
{
    int nrOfHosts = 3;
    int userChoice;
    printf("Available hosts: \n");
    char hosts[3][20] = { "Local",
                          "Andreas",
                          "Alex"
                        };

    //printf("%s\n", hosts[0]);
    //printf("%s\n", hosts[1]);
    for(int i = 0; i < nrOfHosts; i++)
    {
        printf("%s [%d]\n", hosts[i], i);
    }


    printf("Connect to IP [0 - 2] or enter a custom IP [3]: ");
    scanf("%d", &userChoice);

    switch (userChoice)
    {
    case 0: 
        strcpy(hostIP, "127.0.0.1");
        break;
    case 1:
        strcpy(hostIP, "78.71.16.247");
        break;
    case 2:
        strcpy(hostIP, "178.78.213.173");
        break;
    default:
        printf("Enter IP: ");
        scanf("%s", hostIP);
        break;
    }
    //fgets(hostIP, sizeof(hostIP), stdin);
}

void renderTestBullets(SDL_Renderer *renderer, Bullet bullets[][MAX_BULLETS], SDL_Texture *testText)
{
    for (int i = 0; i < MAX_PLAYERS; i++)
        for(int j = 0; j < MAX_BULLETS; j++)
            if(isBulletActive(bullets[i][j]))
                SDL_RenderCopy(renderer, testText, NULL, getBulletRect(bullets[i][j]));
}