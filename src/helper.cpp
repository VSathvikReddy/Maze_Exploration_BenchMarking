#include "../Include/helper.hpp"
#include "../Include/player.hpp"
#include "../Include/tile.hpp"
#include "../Include/values.hpp"
#include <fstream>

void update(Player& player, const std::vector<int>& maze, TileMap& display){
    int px = player.x;
    int py = player.y;

    // Offsets for center, up, down, left, right
    const std::vector<int> dx = {0,  1, -1, 0, 0, 1, 1, -1, -1};
    const std::vector<int> dy = {0,  0,  0, 1,-1, 1, -1, 1, -1};

    for (int k = 0; k < 5; k++) { //AdjCheck
        int x = px + dx[k];
        int y = py + dy[k];

        // bounds check
        if (x < 0 || x >= width || y < 0 || y >= height)
            continue;
        if(k>4 && maze[py* width + x] == '1' && maze[y*width + x] == '1'){ //Peak throuh wall check
            continue;
        }
        // reveal that tile
        int tileNumber = maze[y * width + x];
        display.make_transparent(x, y);
        player.memory[y * width + x] = tileNumber;
    }
}

std::vector<int> loadMaze(const std::string& filename,unsigned &width,unsigned &height){
    std::ifstream file(filename);
    if (!file.is_open()) {
        return {}; // file error
    }

    std::string line;
    std::vector<int> out;out.reserve(width*height);

    height =0;width = 0;
    while (std::getline(file, line)){
        // Remove trailing '\r' if present (Windows CRLF)
        if(!line.empty()&& line.back() == '\r')
            line.pop_back();

        unsigned new_width = 0;
        for(char c: line){
            if(!(c>='0'&&c<='9')) continue;
            new_width++;
            out.push_back(c - '0');
        }
        if(new_width == 0){continue;}
        if(width==0){ width=new_width;}
        else if(width!=new_width){ return {};}

        height++;
    }

    if(height==0){ return {};}

    return out;
}

void game_loop(Player &player,long long time,bool &reached){
    if(player.memory[player.x + player.y*width]==3){ reached = true;}
    std::pair<int,int> step;
    if(reached){ step = player.gotoTarget(0,0);}
    else{step = player.think();} 
    int x = step.first,y = step.second;
    if((x*x) + (y*y) >1){ return;} //Just need to check length, wall and all .move takes care of
    player.move(x,y);
}
