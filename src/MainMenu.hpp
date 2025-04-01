#pragma once

#include "raylib.h"
#include "Scene.hpp"
#include "Animator.hpp"
#include <string>


class MainMenu : public Scene
{
public:
    MainMenu();
    std::string update() override;
    void draw() override;
    void onSwitch() override;
    void onExit() override;
    ~MainMenu() override = default;

private:
};
