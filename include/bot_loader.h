#pragma once
#include <vector>
#include <string>
#include <memory>
#include <filesystem>

#include "player.h"

class BotLoader{
private:
    std::vector<std::string> bot_paths;
    size_t current_index = 0;

    std::unique_ptr<Player_base> load_bot(const std::string& path,uint8_t tile_size, int x, int y);
public:
    explicit BotLoader(const std::string& folder);

    bool has_next() const;
    std::unique_ptr<Player_base> next(uint8_t tile_size, int x, int y);

    void debug() const;

};