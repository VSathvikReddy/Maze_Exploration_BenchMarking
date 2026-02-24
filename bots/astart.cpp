#include "player.h"
#include <vector>
#include <map>
#include <queue>
#include <cmath>

// A simple structure to hold our A* node data
struct Node {
    sf::Vector2i pos;
    int g; // Exact cost from start to this node
    int f; // Estimated total cost (g + heuristic)
    int h; // Exact heuristic distance to goal (used for tie-breaking)
    
    // We want the priority queue to act as a Min-Heap (lowest f-cost on top)
    bool operator>(const Node& other) const {
        if (f == other.f) {
            // TIE-BREAKER: If f-costs are tied, prioritize the node physically closer to the goal
            return h > other.h; 
        }
        return f > other.f;
    }
};

class AStarBot : public Player_base {
private:
    std::map<std::pair<int, int>, uint8_t> grid; // 0 = known floor/unknown, 1 = known wall
    std::vector<sf::Vector2i> current_path;
    sf::Vector2i goal_pos;
    uint8_t t_size;

    bool is_walkable(uint8_t tile) {
        return tile == static_cast<uint8_t>(TileType::FLOOR) || 
               tile == static_cast<uint8_t>(TileType::GOAL);
    }

    // Translates the target coordinate step into a physical direction 
    // using your custom inverted axes
    Direction get_dir(int from_x, int from_y, int to_x, int to_y) {
        if (to_y == from_y + 1) return Direction::UP;
        if (to_x == from_x - 1) return Direction::RIGHT;
        if (to_y == from_y - 1) return Direction::DOWN;
        if (to_x == from_x + 1) return Direction::LEFT;
        return Direction::NONE;
    }

    // Heuristic: Manhattan Distance (perfect for a 4-way grid)
    int heuristic(int x, int y) {
        return std::abs(goal_pos.x - x) + std::abs(goal_pos.y - y);
    }

    // Calculates the shortest path assuming unknown tiles are walkable floors
    std::vector<sf::Vector2i> calculate_astar(sf::Vector2i start) {
        std::priority_queue<Node, std::vector<Node>, std::greater<Node>> open_set;
        std::map<std::pair<int, int>, int> g_score;
        std::map<std::pair<int, int>, sf::Vector2i> parent;

        int start_h = heuristic(start.x, start.y);
        open_set.push({start, 0, start_h, start_h});
        g_score[{start.x, start.y}] = 0;

        while (!open_set.empty()) {
            Node current = open_set.top();
            open_set.pop();

            // If we reached the goal, trace the path backwards
            if (current.pos.x == goal_pos.x && current.pos.y == goal_pos.y) {
                std::vector<sf::Vector2i> path;
                sf::Vector2i curr = current.pos;
                while (curr.x != start.x || curr.y != start.y) {
                    path.push_back(curr);
                    curr = parent[{curr.x, curr.y}];
                }
                return path; // Path is reversed (target -> start), perfect for popping off the back
            }

            // Using your engine's inverted axes
            std::vector<sf::Vector2i> neighbors = {
                {current.pos.x, current.pos.y + 1}, // UP
                {current.pos.x - 1, current.pos.y}, // RIGHT
                {current.pos.x, current.pos.y - 1}, // DOWN
                {current.pos.x + 1, current.pos.y}  // LEFT
            };

            for (auto n : neighbors) {
                // OPTIMISTIC ASSUMPTION: If it's not strictly known as a wall (1), treat it as a floor (0)
                if (grid.find({n.x, n.y}) != grid.end() && grid[{n.x, n.y}] == 1) {
                    continue; // Skip known walls
                }

                int tentative_g = g_score[{current.pos.x, current.pos.y}] + 1;

                if (g_score.find({n.x, n.y}) == g_score.end() || tentative_g < g_score[{n.x, n.y}]) {
                    parent[{n.x, n.y}] = current.pos;
                    g_score[{n.x, n.y}] = tentative_g;
                    
                    int h_score = heuristic(n.x, n.y);
                    int f_score = tentative_g + h_score;
                    open_set.push({n, tentative_g, f_score, h_score}); // Push with all 4 Node parameters
                }
            }
        }
        return {}; // No path found
    }
// Override the SFML draw method
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override 
    {
        // 1. Call the base class draw so the bot sprite still renders
        Player_base::draw(target, states);

        // 2. If we don't have a path, nothing to draw
        if (current_path.empty()) return;

        // 3. Create a line strip (path size + 1 to connect to the bot's center)
        sf::VertexArray lines(sf::LinesStrip, current_path.size() + 1);
        float half_tile = t_size / 2.0f;

        // 4. Start the line exactly at the bot's current physical position
        sf::Vector2i cur_pos = get_position(); 
        
        // NO NEGATIVE SIGNS: Use the raw logical coordinates!
        float start_px = cur_pos.x * t_size + half_tile;
        float start_py = cur_pos.y * t_size + half_tile;

        lines[0].position = sf::Vector2f(start_px, start_py);
        lines[0].color = sf::Color::Red; 

        // 5. Draw the rest of the path
        int index = 1;
        for (auto it = current_path.rbegin(); it != current_path.rend(); ++it) {
            
            // NO NEGATIVE SIGNS here either!
            float px = it->x * t_size + half_tile;
            float py = it->y * t_size + half_tile;
            
            lines[index].position = sf::Vector2f(px, py);
            lines[index].color = sf::Color::Red;
            
            index++;
        }

        // 6. Render the line
        target.draw(lines, states);
    }
public:
    AStarBot(uint8_t tile_size, int w, int h)
        : Player_base(tile_size, 0, 0) // Explicitly start at 0, 0
    {
        t_size = tile_size; // Save for rendering
        name = "A*";
        
        // Calculate the goal coordinates dynamically.
        // Because RIGHT maps to x-1 and DOWN maps to y-1, 
        // the bottom right corner is -(w-1) and -(h-1).
        goal_pos = {(w - 1), (h - 1)}; 
    }

    Direction intent_to_move(sf::Vector2i cur_position, uint8_t top, uint8_t right, uint8_t down, uint8_t left) override 
    {
        int cx = cur_position.x;
        int cy = cur_position.y;

        // 1. Update mental map with absolute facts
        grid[{cx, cy + 1}] = is_walkable(top) ? 0 : 1;
        grid[{cx - 1, cy}] = is_walkable(right) ? 0 : 1;
        grid[{cx, cy - 1}] = is_walkable(down) ? 0 : 1;
        grid[{cx + 1, cy}] = is_walkable(left) ? 0 : 1;

        // 2. Did a newly discovered wall break our current planned path?
        bool path_blocked = false;
        if (!current_path.empty()) {
            // Because the path vector is stored in reverse, the NEXT step is at the back
            sf::Vector2i next_step = current_path.back();
            if (grid[{next_step.x, next_step.y}] == 1) {
                path_blocked = true;
            }
        }

        // 3. Recalculate A* if we have no path or our current path was blocked
        if (current_path.empty() || path_blocked) {
            current_path = calculate_astar(cur_position);
        }

        // 4. Take the next physical step along the path
        if (!current_path.empty()) {
            sf::Vector2i next_step = current_path.back();
            current_path.pop_back();
            return get_dir(cx, cy, next_step.x, next_step.y);
        }

        return Direction::NONE; // The bot is completely trapped or has already reached the goal!
    }
};

extern "C"
Player_base* create_bot(uint8_t tile_size, int w, int h) {
    return new AStarBot(tile_size, w, h);
}