#pragma once
#include <memory>
#include <string>
#include "player.h"

std::unique_ptr<Player_base> load_bot(const std::string& path,uint8_t tile_size, int x, int y);