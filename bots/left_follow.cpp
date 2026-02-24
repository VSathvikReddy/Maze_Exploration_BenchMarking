#include "player.h"

class LeftWallBot : public Player_base {
private:
    Direction facing; // The only "memory" this bot needs!

    bool is_walkable(uint8_t tile) {
        return tile == static_cast<uint8_t>(TileType::FLOOR) || 
               tile == static_cast<uint8_t>(TileType::GOAL);
    }

    // Helper to figure out absolute directions based on where we are facing
    Direction get_left_of(Direction dir) {
        if (dir == Direction::UP) return Direction::LEFT;
        if (dir == Direction::LEFT) return Direction::DOWN;
        if (dir == Direction::DOWN) return Direction::RIGHT;
        if (dir == Direction::RIGHT) return Direction::UP;
        return Direction::NONE;
    }

    Direction get_right_of(Direction dir) {
        if (dir == Direction::UP) return Direction::RIGHT;
        if (dir == Direction::RIGHT) return Direction::DOWN;
        if (dir == Direction::DOWN) return Direction::LEFT;
        if (dir == Direction::LEFT) return Direction::UP;
        return Direction::NONE;
    }

    Direction get_back_of(Direction dir) {
        if (dir == Direction::UP) return Direction::DOWN;
        if (dir == Direction::DOWN) return Direction::UP;
        if (dir == Direction::LEFT) return Direction::RIGHT;
        if (dir == Direction::RIGHT) return Direction::LEFT;
        return Direction::NONE;
    }

    // Checks if a specific absolute direction is currently walkable
    bool can_move(Direction dir, uint8_t top, uint8_t right, uint8_t down, uint8_t left) {
        if (dir == Direction::UP) return is_walkable(top);
        if (dir == Direction::RIGHT) return is_walkable(right);
        if (dir == Direction::DOWN) return is_walkable(down);
        if (dir == Direction::LEFT) return is_walkable(left);
        return false;
    }

public:
    LeftWallBot(uint8_t tile_size, int x, int y)
        : Player_base(tile_size, x, y)
    {
        name = "LeftWall";
        facing = Direction::UP; // Arbitrary starting direction
    }

    Direction intent_to_move(sf::Vector2i cur_position, uint8_t top, uint8_t right, uint8_t down, uint8_t left) override 
    {
        // 1. Check our relative LEFT
        Direction relative_left = get_left_of(facing);
        if (can_move(relative_left, top, right, down, left)) {
            facing = relative_left; // Turn left
            return facing;          // Step forward
        }

        // 2. Check relative STRAIGHT
        if (can_move(facing, top, right, down, left)) {
            return facing; // Keep facing the same way and step forward
        }

        // 3. Check relative RIGHT
        Direction relative_right = get_right_of(facing);
        if (can_move(relative_right, top, right, down, left)) {
            facing = relative_right; // Turn right
            return facing;           // Step forward
        }

        // 4. Dead end! Do a U-Turn (relative BACK)
        facing = get_back_of(facing);
        return facing;
    }
};


extern "C"
Player_base* create_bot(uint8_t tile_size, int x, int y) {
    return new LeftWallBot(tile_size, x, y);
}