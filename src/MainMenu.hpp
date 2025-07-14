#pragma once

#include "raylib.h"
#include "Scene.hpp"
#include "Animator.hpp"
#include "Button.hpp"
#include <string>
#include <array>

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

    std::array<Button, 4> m_buttons;
    bool exitAnimationStarted;
    Music homeMusic;
};
