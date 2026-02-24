#include "game.h"

#include <iostream>
#include <thread>
#include <chrono>

#include <stdexcept>
Game::Game(const std::string& maze_location, const std::string& tileset_location, const std::string& bots_location,uint8_t tile_size):
    maze(maze_location, tileset_location, tile_size),
    loader(bots_location),
    window(sf::VideoMode({static_cast<unsigned int>(maze.getWidth()*tile_size),static_cast<unsigned int>(maze.getHeight()*tile_size)}), "Tilemap")
    {
}

Game::~Game(){
}

void Game::handle_event(const sf::Event& event){
    if (event.type == sf::Event::Closed){
        window.close();
    }if (event.type == sf::Event::KeyPressed){
        if(event.key.code == sf::Keyboard::Escape){
            window.close();
        }
    }
}

void Game::render(){
    window.clear();

    window.draw(maze);
    window.draw(*player);

    window.display();
}


bool Game::update(){
    sf::Vector2i pos = player->get_position();

    Direction move_direction = player->intent_to_move(pos,
        maze.getTileValue(pos.x, pos.y + 1), // top
        maze.getTileValue(pos.x - 1, pos.y), // right
        maze.getTileValue(pos.x, pos.y - 1), // down
        maze.getTileValue(pos.x + 1, pos.y)  // left
    );

    if(move_direction == Direction::NONE){
        return false;
    }

    sf::Vector2i new_position = player->get_position();
    switch (move_direction) {
        case Direction::UP:
            new_position.y += 1;
            break;
        case Direction::RIGHT:
            new_position.x -= 1;
            break;
        case Direction::DOWN:
            new_position.y -= 1;
            break;
        case Direction::LEFT:
            new_position.x += 1;
            break;
        default:
            break;
    }
    uint8_t tile_type = maze.getTileValue(new_position.x, new_position.y);

    if(tile_type == static_cast<uint8_t>(TileType::WALL) || tile_type == static_cast<uint8_t>(TileType::ERROR)){
        return false;
    }

    player->set_position(new_position);
    if(tile_type == static_cast<uint8_t>(TileType::GOAL)){
        return true;
    }
    return false;

}
#define GREEN_ANSI "\033[32m"
#define YELLOW_ANSI "\033[33m"
#define RED_ANSI  "\033[31m"
#define RESET_ANSI "\033[0m"

void Game::skip_bot(){
    window.clear();window.display();
    std::cout<<GREEN_ANSI<<"["<<player->name<<"]"<<RESET_ANSI<<':'<<RED_ANSI<<" Skipped"<<RESET_ANSI<<'.'<<std::endl;
    
    std::this_thread::sleep_for(std::chrono::milliseconds(500)); 
}

void Game::finish_bot(){
    render();
    sf::Time elapsed = clock.getElapsedTime();
    std::cout<<GREEN_ANSI<<"["<<player->name<<"]"<<RESET_ANSI<<": "<<YELLOW_ANSI<<elapsed.asSeconds()<<RESET_ANSI<<"s."<<std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(500)); 
}

void Game::test_player(){
    clock.restart();
    while (window.isOpen()){

        sf::Event event;
        while(window.pollEvent(event)){
            handle_event(event);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
            skip_bot();
            return;
        }


        if(update()){
            finish_bot();
            return;
        }
        //std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
        render();

    }
}

void Game::run(){
    while(loader.has_next()){
        player = loader.next(maze.getTileSize(),maze.getWidth(),maze.getHeight());
        if (!player){
            throw std::runtime_error("Player failed to load\n");
        }
        test_player(); 
    }
    
}