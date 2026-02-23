#include <iostream>
#include <optional>

#include "maze.h"
#include "grid.h"


int main(){
    Maze maze("assets/maze.txt", NO_TILESET, 32);

    sf::RenderWindow window(sf::VideoMode({static_cast<unsigned int>(maze.getWidth()*32), static_cast<unsigned int>(maze.getHeight()*32)}), "Tilemap");

    while (window.isOpen()){
        // handle events
        sf::Event event;
        while (window.pollEvent(event)){
            if(event.type == sf::Event::Closed)
                window.close();
        }

        // draw the map
        window.clear();
        window.draw(maze);
        window.display();
    }
    
    return 0;
}