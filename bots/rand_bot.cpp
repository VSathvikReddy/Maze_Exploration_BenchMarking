#include "player.h"

#include <random>

class RandomBot : public Player_base {
public:
    RandomBot(uint8_t tile_size,int x, int y)
        : Player_base(tile_size, x, y)
    {
        name = "RandomBot";
    }

    Direction intent_to_move(sf::Vector2i cur_position,uint8_t top, uint8_t right, uint8_t down, uint8_t left) override{
        std::vector<uint8_t> surs = {top,right,down,left};
        int r = 0;
        do{
            r = rand() % 4;
        }while(surs[r] != static_cast<uint8_t>(TileType::FLOOR));
        return static_cast<Direction>(r);
    }
};

extern "C"
Player_base* create_bot(uint8_t tile_size, int x, int y) {
    return new RandomBot(tile_size,x,y);
}