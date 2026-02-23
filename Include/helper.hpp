#ifndef HELPER_HPP
#define HELPER_HPP
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

// Forward declarations to avoid circular includes
class Player;
class TileMap;

// Reveals the surrounding tiles around the player
void update(Player& player,
            const std::vector<int>& maze,
            TileMap& display);

// Loads a maze text file of digits (0–9).
// Sets width & height; returns {} on error.
std::vector<int> loadMaze(const std::string& filename,
                          unsigned& width,
                          unsigned& height);

void game_loop(Player &player,long long time,bool &reached);
#endif
