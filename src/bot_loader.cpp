#include "bot_loader.h"
#include "loader.h"
#include <algorithm>
#include <iostream>

namespace fs = std::filesystem;

#ifdef _WIN32
    constexpr const char* LIB_EXT = ".dll";
#else
    constexpr const char* LIB_EXT = ".so";
#endif

BotLoader::BotLoader(const std::string& folder){
    if(!fs::exists(folder)) {
        throw std::runtime_error("Bot folder does not exist\n");
        return;
    }

    for(const auto& entry:fs::directory_iterator(folder)){
        if(entry.path().extension() == LIB_EXT){
            bot_paths.push_back(entry.path().string());
        }
    }
    std::sort(bot_paths.begin(), bot_paths.end());
}

bool BotLoader::has_next() const{
    return current_index < bot_paths.size();
}

std::unique_ptr<Player_base>BotLoader::next(uint8_t tile_size){
    if (!has_next())
        return nullptr;

    const std::string& path =bot_paths[current_index++];

    return load_bot(path, tile_size);
}

void BotLoader::debug() const{
    if (bot_paths.empty()) {
        std::cout << "No bot modules found.\n";
        return;
    }

    std::cout << "Discovered bot modules:\n";

    for (size_t i = 0; i < bot_paths.size(); ++i)
    {
        std::cout << "[" << i << "] "
                  << bot_paths[i] << "\n";
    }
}