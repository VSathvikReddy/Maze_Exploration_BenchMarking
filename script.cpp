#include "Include/player.hpp"

std::pair<int,int> back(const std::pair<int,int>& p){
    return {-p.first, -p.second};
}

class Player_Derived: public Player{
private:
//Feel free to define your own functions and members, here to implement your algo, like previous here
std::pair<int,int> previous;

public:
    using Player::Player; //Inheriting constructors, don't touch

    // Use these as see fit, 
    // std::vector<int> memory; Stores the maze at is is explored, 0 unexplored, 1 walkable, 2 wall, the tile at (x,y) is x + y*width.

    std::pair<int,int> gotoTarget(int target_x, int target_y){
        return Player::gotoTarget(target_x,target_y);
    }


    std::pair<int,int> think(long long time) override{
        auto send = Player::think(time);
        for(int i=0;i<10&&previous==send;i++){
            send = Player::think();
        }
        previous = back(send);
        return send;
    }

};