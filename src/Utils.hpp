#pragma once
#include "raylib.h"
#include <array>
#include <string>

#define AUGMENTS_POOL_SIZE 9

namespace utils
{
    extern Camera2D gameCamera;
    extern std::array<std::array<std::string, 2>, AUGMENTS_POOL_SIZE> augmentPool;
};