#ifndef GAME_H
#define GAME_H

#include "maze.h"
#include "player.h"
#include "bot_loader.h"

class Game{
private:
    Maze maze;
    BotLoader loader;
    std::unique_ptr<Player_base> player;
    sf::RenderWindow window;
    
    sf::Clock clock;

    void handle_event(const sf::Event& event);
    bool update();
    void render();

    void test_player();
    void send_feedback();
public:
    Game(const std::string& maze_location, const std::string& tileset_location,const std::string& bots_location, uint8_t tile_size);
    ~Game();

    
    void run();

};

#endif