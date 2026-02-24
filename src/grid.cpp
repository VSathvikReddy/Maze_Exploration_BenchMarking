#include "grid.h"
#include <cassert>
#include <ostream>

Grid::Grid(int w, int h, const std::vector<uint8_t>& data) : width(w), height(h), cells(data) {
    assert( static_cast<std::size_t>(w*h) == data.size() && "Data size must match width * height" );
}

uint8_t* Grid::operator[](int y) {
    return &cells[y * width];
}
const uint8_t* Grid::operator[](int y) const {
    return &cells[y * width];
}

int Grid::getWidth() const { return width; }
int Grid::getHeight() const { return height; }

std::ostream& operator<<(std::ostream& os, const Grid& grid) {
    for (int y = 0; y < grid.height; ++y) {
        for (int x = 0; x < grid.width; ++x) {

            // The static_cast<int> is perfect here to prevent printing ASCII gibberish!
            os << static_cast<int>(grid[y][x]) << " ";
        }
        os << "\n";
    }
    return os;
}