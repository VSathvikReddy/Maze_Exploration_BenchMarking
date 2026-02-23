#ifndef TILE_HPP
#define TILE_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "values.hpp"

class TileMap : public sf::Drawable, public sf::Transformable {
private:
    sf::VertexArray m_vertices;
    sf::Texture m_tileset;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void check_neighbour(int x, int y, const std::vector<int> &tiles, int &dir, float &tx);
public:
    TileMap();
    ~TileMap();

    bool loadFromFile(const std::string& address);
    bool loadFromImage(const sf::Image& img);

    void set_tiles_in_map();

    bool set_textures(const std::vector<int>& tiles);

    bool set_tile(int x, int y,const std::vector<int> &tiles);
    bool set_tile(int x, int y,const std::vector<int> &tiles, int tilesPerCol);
    bool make_transparent(int x, int y);
};

#endif
