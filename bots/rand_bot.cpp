#include "player.h"
#include <random>

class RandomBot : public Player_base {
public:
    RandomBot(uint8_t tile_size)
        : Player_base(tile_size)
    {
        name = "RandomBot";
    }

    Direction intent_to_move() override {
        int r = rand() % 4;
        return static_cast<Direction>(r);
    }
};

extern "C"
Player_base* create_bot(uint8_t tile_size) {
    return new RandomBot(tile_size);
}