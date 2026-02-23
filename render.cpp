#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <SFML/Graphics.hpp>

#include "Include/values.hpp"
#include "Include/tile.hpp"
#include "Include/player.hpp"
#include "Include/helper.hpp"

#include "script.cpp"

int main(){
    //Load Maze
    std::vector<int> maze = loadMaze(maze_address,width,height);
    if(maze.size()==0){ return -1;}

    //Initialize the window
    int pixel_height = pixel*height;
    int pixel_width = pixel*width;
    sf::RenderWindow window(sf::VideoMode(pixel_width, pixel_height), "Pixel Movement");
    //window.setFramerateLimit(60);

    Player_Derived sathvik(player_texture);

    TileMap map;
    if((!map.loadFromFile(tile_texture))||(!map.set_textures(sathvik.memory))) return -1;
    TileMap actual;
    if((!actual.loadFromFile(tile_texture))||(!actual.set_textures(maze))) return -1;

    update(sathvik,maze,map);

    sf::Clock clock;
    sf::Time time_elapsed = sf::seconds(0.f);  
    clock.restart();

    long long game_ticks = 0, path_length=0;
    bool paused = false,reached = false;
    while(window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                std::cout<<"Your game took:"<<game_ticks<<" to complete"<<std::endl;
                std::cout<<"The shortest path you found is of length: "<<path_length<<" to complete"<<std::endl;
                window.close();
            }
            else if(event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Space) {
                    paused = !paused;
                }else if(event.key.code == sf::Keyboard::Right){
                    life_speed = life_speed*0.5f;
                }else if(event.key.code == sf::Keyboard::Left){
                    life_speed = life_speed*2.0f;
                }
            }
        }
        time_elapsed+= clock.restart();
        if (time_elapsed >= life_speed && !paused){
            if(reached){ path_length++;}
            game_ticks++;
            game_loop(sathvik,game_ticks,reached);
            update(sathvik,maze,map);
            time_elapsed = sf::seconds(0.f);
            if(reached && sathvik.x == 0 && sathvik.y ==0){ paused =true;}
        }

        window.clear(sf::Color::Black);
        window.draw(actual);
        window.draw(map);
        
        window.draw(sathvik.sprite);
        window.display();

    }
    return 0;
}
