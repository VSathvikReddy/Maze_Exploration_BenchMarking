#include <iostream>
#include <optional>

#include "maze.h"
#include "player.h"
#include "game.h"


int main(){
    Game game("assets/maze.txt", NO_TILESET,"bots", 32);
    game.run();

    return 0;
}