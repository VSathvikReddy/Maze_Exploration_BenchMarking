#include "bot_loader.h"
#include <algorithm>
#include <iostream>
#include <stdexcept>
namespace fs = std::filesystem;

#ifdef _WIN32
    #include <windows.h>
    constexpr const char* LIB_EXT = ".dll";
#else
    #include <dlfcn.h>
    constexpr const char* LIB_EXT = ".so";
#endif

BotLoader::BotLoader(const std::string& folder){
    std::string bots_build_folder = "build/"+folder; 
    if(!fs::exists(bots_build_folder)) {
        throw std::runtime_error("Bot folder does not exist\n");
        return;
    }

    for(const auto& entry:fs::directory_iterator(bots_build_folder)){
        if(entry.path().extension() == LIB_EXT){
            bot_paths.push_back(entry.path().string());
        }
    }
    std::sort(bot_paths.begin(), bot_paths.end());
}

bool BotLoader::has_next() const{
    return current_index < bot_paths.size();
}

std::unique_ptr<Player_base>BotLoader::next(uint8_t tile_size, int x, int y){
    if (!has_next())
        return nullptr;

    return load_bot(bot_paths[current_index++], tile_size, x, y);
}

void BotLoader::debug() const{
    if (bot_paths.empty()) {
        std::cout<<"No bot modules found."<<std::endl;
        return;
    }

    std::cout<<"Discovered bot modules:\n";

    for(int i=0;i<bot_paths.size();i++){
        std::cout<<"["<<i<<"] "<<bot_paths[i]<<"\n";
    }std::cout<<std::endl;
}















using CreateBotFunc = Player_base* (*)(uint8_t,int,int);

std::unique_ptr<Player_base> BotLoader::load_bot(const std::string& path,uint8_t tile_size, int x, int y){
#ifdef _WIN32
    HMODULE lib = LoadLibraryA(path.c_str());
    if (!lib) {
        std::cerr << "Failed to load DLL\n";
        return nullptr;
    }

    auto create =
        (CreateBotFunc)GetProcAddress(lib, "create_bot");

#else
    void* lib = dlopen(path.c_str(), RTLD_NOW);
    if (!lib) {
        std::cerr << dlerror() << "\n";
        return nullptr;
    }

    CreateBotFunc create =(CreateBotFunc)dlsym(lib, "create_bot");
#endif

    if (!create) {
        std::cerr << "create_bot not found\n";
        return nullptr;
    }

    return std::unique_ptr<Player_base>(create(tile_size, x, y));
}