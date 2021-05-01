#include "networkgamestate.h"

#define PUBLIC

// struct Networkgamestate_type {
//     Networkplayer players[5];
//     Networkbullet bullets[100];
// };

PUBLIC Networkgamestate createNetworkgamestate()
{
    Networkgamestate a;
    for(int i = 0; i < MAX_PLAYERS; i++) a.players[i] = createNetworkplayer(-1);
    // sizeof(*a);
    return a;
}

// PUBLIC Networkplayer* getNetworkgamestateplayer(Networkgamestate *a, int playerID)
// {
//     return &(*a)->players[playerID];
// }
