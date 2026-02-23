#ifndef GAME_H
#define GAME_H

#include "maze.h"
#include "player.h"

class Game{
private:
    Maze maze;
    std::unique_ptr<Player_base> player;
    sf::RenderWindow window;
    
    sf::Clock clock;

    void handle_event(const sf::Event& event);
    bool update();
    void render();
public:
    Game(const std::string& maze_location, const std::string& tileset_location, uint8_t tile_size);
    ~Game();

    void test_player();

};

#endif