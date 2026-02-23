#include "../Include/player.hpp"
#include <algorithm>
#include <random>

Player::Player() {
    memory = std::vector<int>(width * height, 0);
}

Player::Player(const std::string& texture_location) : Player() {
    texture.loadFromFile(texture_location, sf::IntRect(0, 0, pixel, pixel));
    sprite.setTexture(texture);
    setPosition(0, 0);
}

Player::~Player() {}

void Player::setPosition(int x, int y) {
    this->x = x;
    this->y = y;
    sprite.setPosition({ x * pixel, y * pixel });
}

void Player::move(int dx, int dy){
    int new_x = x + dx;
    int new_y = y + dy;

    if (new_x < 0 || new_x >= width) return;
    if (new_y < 0 || new_y >= height) return;

    int index = new_x + new_y * width;
    if (memory[index] == 2) return;

    setPosition(new_x, new_y);
}

std::pair<int,int> Player::gotoTarget(int target_x, int target_y) { //Chatgpted, weill make my own
    int sx = x;
    int sy = y;

    // If we are already at the target
    if (sx == target_x && sy == target_y)
        return {0,0};

    // If target is outside memory bounds or is a wall
    if (target_x < 0 || target_x >= width || 
        target_y < 0 || target_y >= height || memory[target_y * width + target_x] == 2 || memory[target_y * width + target_x] == 0)
        return {0,0};

    // BFS structures
    std::vector<int> dist(width * height, -1);
    std::vector<std::pair<int,int>> parent(width * height, {-1,-1});
    std::queue<std::pair<int,int>> q;

    q.push({sx, sy});
    dist[sy * width + sx] = 0;

    const int DX[4] = { 1, -1, 0, 0 };
    const int DY[4] = { 0, 0, 1, -1 };

    // BFS loop
    while (!q.empty()) {
        auto [cx, cy] = q.front();
        q.pop();

        if (cx == target_x && cy == target_y)
            break;

        for (int d = 0; d < 4; d++) {
            int nx = cx + DX[d];
            int ny = cy + DY[d];

            // bounds
            if (nx < 0 || nx >= width || ny < 0 || ny >= height)
                continue;

            int idx = ny * width + nx;

            if (memory[idx] == 2) // wall
                continue;

            if (memory[idx] == 0) // unknown
                continue; // cannot walk through unknown

            if (dist[idx] != -1) // already visited
                continue;

            dist[idx] = dist[cy * width + cx] + 1;
            parent[idx] = {cx, cy};
            q.push({nx, ny});
        }
    }

    // Cannot reach target with known memory
    if (dist[target_y * width + target_x] == -1)
        return {0,0};

    // Walk backwards from target until we reach one step ahead of (sx, sy)
    int tx = target_x;
    int ty = target_y;

    while (!(parent[ty * width + tx].first == sx &&
             parent[ty * width + tx].second == sy))
    {
        auto p = parent[ty * width + tx];
        tx = p.first;
        ty = p.second;
    }

    // Return the movement delta
    int dx = tx - sx;
    int dy = ty - sy;

    return {dx, dy};
}



std::pair<int,int> Player::think(long long time){ //ChatGpt algoritm for random walk, because I was tiered af

    int px = x;
    int py = y;

    // 4 directions
    std::vector<std::pair<int,int>> dirs = {
        { 1,  0},   // right
        {-1,  0},   // left
        { 0,  1},   // down
        { 0, -1}    // up
    };

    // shuffle random directions
    static std::mt19937 rng(std::random_device{}());
    std::shuffle(dirs.begin(), dirs.end(), rng);

    // --- STEP 1: Explore unknown tiles first ---
    for (auto& d : dirs) {
        int nx = px + d.first;
        int ny = py + d.second;

        if (nx < 0 || nx >= width) continue;
        if (ny < 0 || ny >= height) continue;

        if (memory[ny * width + nx] == 0) {
            return d;
        }
    }

    // --- STEP 2: Move to any walkable tile (1) ---
    for (auto& d : dirs) {
        int nx = px + d.first;
        int ny = py + d.second;

        if (nx < 0 || nx >= width) continue;
        if (ny < 0 || ny >= height) continue;

        if ((memory[ny * width + nx] != 2)) {
            return d;
        }
    }

    // --- STEP 3: No move available → stay still ---
    return {0,0};
}


