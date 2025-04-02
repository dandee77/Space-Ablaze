#pragma once

#include "raylib.h"
#include <string>

#define GAME_SCREEN_WIDTH 3840
#define GAME_SCREEN_HEIGHT 2160


class Scene
{
public:

    Scene() = default;
    virtual std::string update() = 0;
    virtual void draw() = 0;
    virtual void onSwitch() = 0;  
    virtual void onExit() = 0;
    virtual ~Scene() = default;
};