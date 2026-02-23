#ifndef GAME_H
#define GAME_H

#include "maze.h"

class Game{
private:
    Maze maze;
    sf::RenderWindow window;

    sf::Clock clock;

public:
    Game(const std::string& maze_location, const std::string& tileset_location, uint8_t tile_size);


};

#endif