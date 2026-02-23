#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <queue>
#include "values.hpp"

class Player{
private:
    sf::Texture texture;

public:
    std::vector<int> memory;
    sf::Sprite sprite;
    int x{};
    int y{};

    Player();
    Player(const std::string& texture_location);
    ~Player();

    void setPosition(int x, int y);
    void move(int dx, int dy);
    virtual std::pair<int,int> gotoTarget(int target_x, int target_y);
    virtual std::pair<int,int> think(long long time=0);
};

#endif
