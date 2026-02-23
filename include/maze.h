#ifndef MAZE_H
#define MAZE_H

#include <string>
#include <iostream>
#include <ostream>

#include <SFML/Graphics.hpp>

#include "grid.h"

#define NO_TILESET ""

enum class TileType: uint8_t{
    WALL = 0,
    FLOOR = 1,
    GOAL = 2,
};

class Maze: public sf::Drawable, public sf::Transformable{
private:
    Grid grid;

    sf::VertexArray m_vertices;

    sf::Texture m_tileset;
    bool m_use_tileset;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    bool load_tileset(const std::string& tileset_location);
    bool load_vertices(uint8_t tile_size);

    void set_tile_texture(sf::Vertex* quad, uint8_t tile_value, uint8_t tile_size);
    void set_tile_position(sf::Vertex* quad, uint8_t x, uint8_t y, uint8_t tile_size);
    void set_tile_color(sf::Vertex* quad, uint8_t tile_value);
public:
    Maze(const std::string& maze_location,const std::string& tileset_location,uint8_t tile_size);
    virtual ~Maze() = default;
    
    Grid loadMazeFromFile(const std::string& maze_location);

    uint8_t getWidth() const;
    uint8_t getHeight() const;
    uint8_t getTileValue(uint8_t x, uint8_t y) const;
};

#endif