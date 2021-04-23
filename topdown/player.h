#ifndef PLAYER_H
#define PLAYER_H
#include <SDL.h>
typedef struct Player_type* Player;


Player createPlayer(int x, int y);
<<<<<<< Updated upstream
void movePlayer(Player p, int up, int down, int right, int left);
SDL_Rect getPlayerRect(Player p);
=======
void movePlayer(Player p, int up, int down, int right, int left, int mouseX, int mouseY);
SDL_Rect* getPlayerRect(Player p);
int getPlayerFrame(Player p);
int getPlayerDirection(Player p);
int getPlayerX(Player p);
int getPlayerY(Player p);

void playerHealth(Player p, int health);
>>>>>>> Stashed changes

#endif
