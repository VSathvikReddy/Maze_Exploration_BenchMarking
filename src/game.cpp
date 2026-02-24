#include "game.h"

Game::Game(const std::string& maze_location, const std::string& tileset_location, const std::string& bots_location,uint8_t tile_size):
    maze(maze_location, tileset_location, tile_size),
    loader(bots_location),
    window(sf::VideoMode({static_cast<unsigned int>(maze.getWidth()*tile_size),static_cast<unsigned int>(maze.getHeight()*tile_size)}), "Tilemap") {

}

Game::~Game(){
}

void Game::handle_event(const sf::Event& event){
    if (event.type == sf::Event::Closed){
        window.close();
    }
}

void Game::render(){
    window.clear();
    window.draw(maze);
    window.draw(*player);
    window.display();
}

void Game::send_feedback(){
    sf::Vector2i pos = player->get_position();

    player->feed_back(pos,
        maze.getTileValue(pos.x, pos.y + 1), // top
        maze.getTileValue(pos.x + 1, pos.y), // right
        maze.getTileValue(pos.x, pos.y - 1), // down
        maze.getTileValue(pos.x - 1, pos.y)  // left
    );
}

bool Game::update(){
    Direction move_direction = player->intent_to_move();
    if(move_direction == Direction::NONE){
        return false;
    }

    sf::Vector2i new_position = player->get_position();
    switch (move_direction) {
        case Direction::UP:
            new_position.y -= 1;
            break;
        case Direction::RIGHT:
            new_position.x += 1;
            break;
        case Direction::DOWN:
            new_position.y += 1;
            break;
        case Direction::LEFT:
            new_position.x -= 1;
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

void Game::test_player(){
    clock.restart();
    while (window.isOpen()){
        // handle events
        sf::Event event;
        while(window.pollEvent(event)){
            handle_event(event);
        }
        if(update()){
            break;
        }
        send_feedback();

        render();

    }
    sf::Time elapsed = clock.getElapsedTime();
    std::cout<<"\033[32m["<<player->name<<"]\033[0m: "<<"\033[33m"<<elapsed.asSeconds()<<"\033[0m s."<<std::endl;
}

void Game::run(){
    while(loader.has_next()){
        player = loader.next(maze.getTileSize(),maze.getWidth(),maze.getHeight());
        if (!player) {
            std::cerr << "Player failed to load"<<std::endl;
        }
        test_player(); 
    }
    
}