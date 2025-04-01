#pragma once
#include "raylib.h"
#include "Scene.hpp"
#include <string>


class LoadingScreen : public Scene
{
public:
    LoadingScreen();
    std::string update() override;
    void draw() override;
    void onSwitch() override;
    void onExit() override;
    ~LoadingScreen() override = default;

private:
    
};