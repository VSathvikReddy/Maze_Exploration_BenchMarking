#include "player.h"
#include <vector>
#include <set>

class DFS : public Player_base {
private:
    std::set<std::pair<int, int>> visited;
    std::vector<sf::Vector2i> path;


    bool is_walkable(uint8_t tile) {
        return tile == static_cast<uint8_t>(TileType::FLOOR) || tile == static_cast<uint8_t>(TileType::GOAL);
    }

public:
    DFS(uint8_t tile_size, int x, int y):Player_base(tile_size, x, y){
        name = "DFS";
    }

    Direction intent_to_move(sf::Vector2i cur_position, uint8_t top, uint8_t right, uint8_t down, uint8_t left) override {
        int cx = cur_position.x;
        int cy = cur_position.y;

        visited.insert({cx, cy});

        // Look for unvisited walkable neighbors
        if (is_walkable(top) && visited.find({cx, cy + 1}) == visited.end()) {
            path.push_back(cur_position);
            return Direction::UP;
        }
        if (is_walkable(right) && visited.find({cx - 1, cy}) == visited.end()) {
            path.push_back(cur_position);
            return Direction::RIGHT;
        }
        if (is_walkable(down) && visited.find({cx, cy - 1}) == visited.end()) {
            path.push_back(cur_position);
            return Direction::DOWN;
        }
        if (is_walkable(left) && visited.find({cx + 1, cy}) == visited.end()) {
            path.push_back(cur_position);
            return Direction::LEFT;
        }

        // 3. No unvisited neighbors → Backtrack
        if (!path.empty()) {
            sf::Vector2i prev = path.back();
            path.pop_back();

            // Calculate which direction to step to get back to 'prev'
            // using your inverted coordinate system mapping
            if (prev.y == cy + 1) return Direction::UP;
            if (prev.x == cx - 1) return Direction::RIGHT;
            if (prev.y == cy - 1) return Direction::DOWN;
            if (prev.x == cx + 1) return Direction::LEFT;
        }

        // 4. If path is empty and no moves exist, we are trapped or done
        return Direction::NONE;
    }
};

extern "C"
Player_base* create_bot(uint8_t tile_size, int x, int y) {
    return new DFS(tile_size, x, y);
}