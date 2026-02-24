#include "player.h"
#include <vector>
#include <set>
#include <map>
#include <queue>

class FloodFill : public Player_base {
private:
    std::set<std::pair<int, int>> visited;       // Tiles we have physically walked on
    std::map<std::pair<int, int>, uint8_t> grid; // Our mental map (0 = floor/goal, 1 = wall)

    bool is_walkable(uint8_t tile) {
        return tile == static_cast<uint8_t>(TileType::FLOOR) || 
               tile == static_cast<uint8_t>(TileType::GOAL);
    }

    Direction get_dir(int from_x, int from_y, int to_x, int to_y) {
        if (to_y == from_y + 1) return Direction::UP;
        if (to_x == from_x - 1) return Direction::RIGHT;
        if (to_y == from_y - 1) return Direction::DOWN;
        if (to_x == from_x + 1) return Direction::LEFT;
        return Direction::NONE;
    }

public:
    FloodFill(uint8_t tile_size, int x, int y)
        : Player_base(tile_size, x, y)
    {
        name = "FloodFill";
    }

    Direction intent_to_move(sf::Vector2i cur_position, uint8_t top, uint8_t right, uint8_t down, uint8_t left) override 
    {
        int cx = cur_position.x;
        int cy = cur_position.y;

        // 1. Update our mental map with current surroundings
        visited.insert({cx, cy});
        grid[{cx, cy}] = 0; 

        if (is_walkable(top)) grid[{cx, cy + 1}] = 0; else grid[{cx, cy + 1}] = 1;
        if (is_walkable(right)) grid[{cx - 1, cy}] = 0; else grid[{cx - 1, cy}] = 1;
        if (is_walkable(down)) grid[{cx, cy - 1}] = 0; else grid[{cx, cy - 1}] = 1;
        if (is_walkable(left)) grid[{cx + 1, cy}] = 0; else grid[{cx + 1, cy}] = 1;

        // 2. GREEDY FILL: If there is an immediate unvisited neighbor, just step into it.
        // This makes the bot naturally "pour" into empty space without complex math.
        if (grid[{cx, cy + 1}] == 0 && visited.find({cx, cy + 1}) == visited.end()) return Direction::UP;
        if (grid[{cx - 1, cy}] == 0 && visited.find({cx - 1, cy}) == visited.end()) return Direction::RIGHT;
        if (grid[{cx, cy - 1}] == 0 && visited.find({cx, cy - 1}) == visited.end()) return Direction::DOWN;
        if (grid[{cx + 1, cy}] == 0 && visited.find({cx + 1, cy}) == visited.end()) return Direction::LEFT;

        // 3. THE WAVEFRONT: We are in a dead-end or fully explored local area.
        // Run a mental search to find the closest known unvisited tile anywhere on the map.
        std::queue<sf::Vector2i> q;
        std::map<std::pair<int, int>, sf::Vector2i> parent;
        std::set<std::pair<int, int>> mental_vis;

        q.push(cur_position);
        mental_vis.insert({cx, cy});

        sf::Vector2i target = {-1, -1};
        bool found = false;

        while (!q.empty()) {
            sf::Vector2i curr = q.front();
            q.pop();

            // Did we find a known floor tile that we haven't physically visited?
            if (visited.find({curr.x, curr.y}) == visited.end()) {
                target = curr;
                found = true;
                break; // Because it's a queue, the first one we find is guaranteed to be the closest
            }

            // Expand outward
            std::vector<sf::Vector2i> neighbors = {
                {curr.x, curr.y + 1}, // UP
                {curr.x - 1, curr.y}, // RIGHT
                {curr.x, curr.y - 1}, // DOWN
                {curr.x + 1, curr.y}  // LEFT
            };

            for (auto n : neighbors) {
                // Only path through known floors
                if (grid.find({n.x, n.y}) != grid.end() && grid[{n.x, n.y}] == 0) {
                    if (mental_vis.find({n.x, n.y}) == mental_vis.end()) {
                        mental_vis.insert({n.x, n.y});
                        parent[{n.x, n.y}] = curr;
                        q.push(n);
                    }
                }
            }
        }

        // 4. EXECUTE: Backtrace from the target to find our immediate next step
        if (found) {
            sf::Vector2i step = target;
            
            // Walk backwards through the parents until we reach the tile right next to us
            while (parent.find({step.x, step.y}) != parent.end()) {
                sf::Vector2i p = parent[{step.x, step.y}];
                if (p.x == cx && p.y == cy) break; // Found our immediate neighbor!
                step = p;
            }
            return get_dir(cx, cy, step.x, step.y);
        }

        // 5. If the search turns up nothing, the maze is 100% explored
        return Direction::NONE;
    }
};

extern "C"
Player_base* create_bot(uint8_t tile_size, int x, int y) {
    return new FloodFill(tile_size, x, y);
}