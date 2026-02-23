#ifndef GRID_H
#define GRID_H

#include <vector>
#include <cstdint>
#include <ostream>

class Grid{
private:
    const int width;
    const int height;
    std::vector<uint8_t> cells;
public:
    Grid(int w, int h, const std::vector<uint8_t>& data);

    uint8_t* operator[](int y);
    const uint8_t* operator[](int y) const;
    
    int getWidth() const;
    int getHeight() const;

    friend std::ostream& operator<<(std::ostream& os, const Grid& grid);
};

#endif