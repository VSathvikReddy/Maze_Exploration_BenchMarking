#include "player.h"
#include <vector>
#include <set>
#include <map>
#include <queue>

class ProperBFS : public Player_base {
private:
    std::queue<sf::Vector2i> bfs_queue;          // Strict FIFO Queue
    std::set<std::pair<int, int>> discovered;    // Tiles added to the queue
    std::set<std::pair<int, int>> visited;       // Tiles we have physically reached
    std::map<std::pair<int, int>, uint8_t> grid; // Known map (0 = floor/goal, 1 = wall)
    
    std::vector<sf::Vector2i> current_path;      // The physical steps to reach the next queued target
    bool initialized = false;

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

    // Standard shortest-path BFS to navigate through the ALREADY KNOWN map
    std::vector<sf::Vector2i> get_path_to_target(sf::Vector2i start, sf::Vector2i target) {
        std::queue<sf::Vector2i> q;
        std::map<std::pair<int, int>, sf::Vector2i> parent;
        std::set<std::pair<int, int>> local_vis;

        q.push(start);
        local_vis.insert({start.x, start.y});

        bool found = false;
        while (!q.empty()) {
            sf::Vector2i curr = q.front();
            q.pop();

            if (curr.x == target.x && curr.y == target.y) {
                found = true;
                break;
            }

            // Using your inverted engine axes
            std::vector<sf::Vector2i> neighbors = {
                {curr.x, curr.y + 1}, // UP
                {curr.x - 1, curr.y}, // RIGHT
                {curr.x, curr.y - 1}, // DOWN
                {curr.x + 1, curr.y}  // LEFT
            };

            for (auto n : neighbors) {
                // We can only walk on floors we know exist
                if (grid.find({n.x, n.y}) != grid.end() && grid[{n.x, n.y}] == 0) {
                    if (local_vis.find({n.x, n.y}) == local_vis.end()) {
                        local_vis.insert({n.x, n.y});
                        parent[{n.x, n.y}] = curr;
                        q.push(n);
                    }
                }
            }
        }

        std::vector<sf::Vector2i> path;
        if (found) {
            sf::Vector2i curr = target;
            while (curr != start) {
                path.push_back(curr);
                curr = parent[{curr.x, curr.y}];
            }
            // Path is currently [target, ..., step1]. 
            // Popping from the back will give us the steps in the correct order.
        }
        return path;
    }

public:
    ProperBFS(uint8_t tile_size, int x, int y)
        : Player_base(tile_size, x, y)
    {
        name = "ProperBFS";
    }

    Direction intent_to_move(sf::Vector2i cur_position, uint8_t top, uint8_t right, uint8_t down, uint8_t left) override 
    {
        int cx = cur_position.x;
        int cy = cur_position.y;

        // 1. Update our mental map with surroundings
        grid[{cx, cy}] = 0; 
        if (is_walkable(top)) grid[{cx, cy + 1}] = 0; else grid[{cx, cy + 1}] = 1;
        if (is_walkable(right)) grid[{cx - 1, cy}] = 0; else grid[{cx - 1, cy}] = 1;
        if (is_walkable(down)) grid[{cx, cy - 1}] = 0; else grid[{cx, cy - 1}] = 1;
        if (is_walkable(left)) grid[{cx + 1, cy}] = 0; else grid[{cx + 1, cy}] = 1;

        // 2. Initialize the start node
        if (!initialized) {
            bfs_queue.push(cur_position);
            discovered.insert({cx, cy});
            initialized = true;
        }

        // 3. If we don't have a path to walk, we need to process the queue
        if (current_path.empty()) {
            
            // We have reached a target tile. Mark it visited.
            visited.insert({cx, cy});

            // Push all undiscovered neighbors into the strict FIFO queue
            // UP
            if (grid[{cx, cy + 1}] == 0 && discovered.find({cx, cy + 1}) == discovered.end()) {
                bfs_queue.push({cx, cy + 1});
                discovered.insert({cx, cy + 1});
            }
            // RIGHT
            if (grid[{cx - 1, cy}] == 0 && discovered.find({cx - 1, cy}) == discovered.end()) {
                bfs_queue.push({cx - 1, cy});
                discovered.insert({cx - 1, cy});
            }
            // DOWN
            if (grid[{cx, cy - 1}] == 0 && discovered.find({cx, cy - 1}) == discovered.end()) {
                bfs_queue.push({cx, cy - 1});
                discovered.insert({cx, cy - 1});
            }
            // LEFT
            if (grid[{cx + 1, cy}] == 0 && discovered.find({cx + 1, cy}) == discovered.end()) {
                bfs_queue.push({cx + 1, cy});
                discovered.insert({cx + 1, cy});
            }

            // Pop the queue until we find the next unvisited target
            sf::Vector2i target = {-1, -1};
            while (!bfs_queue.empty()) {
                sf::Vector2i front = bfs_queue.front();
                bfs_queue.pop();
                
                if (visited.find({front.x, front.y}) == visited.end()) {
                    target = front;
                    break;
                }
            }

            if (target.x == -1) {
                return Direction::NONE; // The maze is fully explored!
            }

            // Calculate the physical route to walk to the next BFS target
            current_path = get_path_to_target(cur_position, target);
        }

        // 4. Follow the physical route step-by-step
        if (!current_path.empty()) {
            sf::Vector2i next_step = current_path.back();
            current_path.pop_back();
            return get_dir(cx, cy, next_step.x, next_step.y);
        }

        return Direction::NONE;
    }
};

extern "C"
Player_base* create_bot(uint8_t tile_size, int x, int y) {
    return new ProperBFS(tile_size, x, y);
}