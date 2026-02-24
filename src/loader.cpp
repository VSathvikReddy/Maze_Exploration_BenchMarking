#include "loader.h"
#include <iostream>
#include <stdexcept>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif

using CreateBotFunc = Player_base* (*)(uint8_t,int,int);

std::unique_ptr<Player_base> load_bot(const std::string& path,uint8_t tile_size, int x, int y){
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