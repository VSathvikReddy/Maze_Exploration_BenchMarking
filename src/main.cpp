#include <iostream>
#include <optional>

#include "maze.h"
#include "player.h"
#include "game.h"


int main(){
    Game game("assets/maze.txt", NO_TILESET, 32);
    game.test_player();

    
    return 0;
}