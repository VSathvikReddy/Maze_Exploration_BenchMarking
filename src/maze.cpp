#include "maze.h"

#include <fstream>
#include <stdexcept>

void Maze::draw(sf::RenderTarget& target, sf::RenderStates states) const{
    states.transform *= sf::Transformable::getTransform();
    if(m_use_tileset){
        states.texture = &m_tileset;
    }
    target.draw(m_vertices, states);
}

bool Maze::load_tileset(const std::string& tileset_location){
    if(tileset_location.empty()){
        m_use_tileset = false;
        return true;
    }
    else if(!m_tileset.loadFromFile(tileset_location)){
        throw std::runtime_error("Could not load tileset: " + tileset_location);
    }
    m_use_tileset = true;
    return true;
}

void Maze::set_tile_texture(sf::Vertex* quad, uint8_t tile_value, uint8_t tile_size){
    int tu = tile_value % (m_tileset.getSize().x / tile_size);
    int tv = tile_value / (m_tileset.getSize().x / tile_size);
    quad[0].texCoords = sf::Vector2f(tu * tile_size, tv * tile_size);
    quad[1].texCoords = sf::Vector2f((tu + 1) * tile_size, tv * tile_size);
    quad[2].texCoords = sf::Vector2f((tu + 1) * tile_size, (tv + 1) * tile_size);
    quad[3].texCoords = sf::Vector2f(tu * tile_size, (tv + 1) * tile_size);
}

void Maze::set_tile_position(sf::Vertex* quad, uint8_t x, uint8_t y, uint8_t tile_size){
    quad[0].position = sf::Vector2f(x * tile_size, y * tile_size);
    quad[1].position = sf::Vector2f((x + 1) * tile_size, y * tile_size);
    quad[2].position = sf::Vector2f((x + 1) * tile_size, (y + 1) * tile_size);
    quad[3].position = sf::Vector2f(x * tile_size, (y + 1) * tile_size);
}

void Maze::set_tile_color(sf::Vertex* quad, uint8_t tile_value){
    TileType type = static_cast<TileType>(tile_value);
    if(type == TileType::WALL){
        quad[0].color = sf::Color::Black;
        quad[1].color = sf::Color::Black;
        quad[2].color = sf::Color::Black;
        quad[3].color = sf::Color::Black;
    }else if (type == TileType::GOAL){
        sf::Color goalColor(255, 223, 0);
        quad[0].color = goalColor;
        quad[1].color = goalColor;
        quad[2].color = goalColor;
        quad[3].color = goalColor;
    }else{
        quad[0].color = sf::Color::White;
        quad[1].color = sf::Color::White;
        quad[2].color = sf::Color::White;
        quad[3].color = sf::Color::White;
    }
}
bool Maze::load_vertices(uint8_t tile_size){
    m_vertices.setPrimitiveType(sf::PrimitiveType::Quads);
    m_vertices.resize(grid.getWidth()*grid.getHeight()*4);

    uint8_t Y = grid.getHeight();
    uint8_t X = grid.getWidth();

    for (uint8_t y = 0; y < Y; ++y) {
        for (uint8_t x = 0; x < X; ++x) {

            sf::Vertex* quad = &m_vertices[(x + y * X) * 4];
            set_tile_position(quad, x, y, tile_size);

            if(m_use_tileset){
                set_tile_texture(quad, grid[y][x], tile_size);
            }else{
                set_tile_color(quad, grid[y][x]);
            }

        }
    }
    return true;
}

Maze::Maze(const std::string& maze_location, const std::string& tileset_location, uint8_t tile_size):grid(loadMazeFromFile(maze_location)){
    load_tileset(tileset_location);
    load_vertices(tile_size);
}

Grid Maze::loadMazeFromFile(const std::string& maze_location) {
    int height, width;
    std::vector<uint8_t> data;

    std::ifstream file(maze_location);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open maze file: " + maze_location);
    }

    file >> width >> height;
    data.reserve(width * height);
    for (int i = 0; i < width * height; ++i) {
        int cell_value;
        file >> cell_value;
        data.push_back(static_cast<uint8_t>(cell_value));
    }
    return Grid(width, height, data);
}


uint8_t Maze::getWidth() const { return grid.getWidth(); }
uint8_t Maze::getHeight() const { return grid.getHeight(); }
uint8_t Maze::getTileValue(uint8_t x, uint8_t y) const { return grid[y][x]; }