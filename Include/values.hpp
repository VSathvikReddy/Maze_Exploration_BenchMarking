#ifndef VALUES_H
#define VALUES_H

#include <SFML/Graphics.hpp>
#include <string>

inline float pixel = 15.f;
inline unsigned width = 20;
inline unsigned height =20;
inline sf::Time life_speed = sf::seconds(0.01f);

inline std::string player_texture = "textures/player_texture.png";
inline std::string tile_texture   = "textures/textures.png";
inline std::string maze_address   = "textures/maze.txt";

inline sf::Vector2u tileSize  = { (unsigned)pixel, (unsigned)pixel };
inline sf::Vector2u windowSize = { width, height };

#endif