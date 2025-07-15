#pragma once

#include "raylib.h"
#include "Scene.hpp"
#include "Animator.hpp"
#include "Button.hpp"
#include <string>
#include <array>
#include "Card.hpp"

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

    Card augmentCard = Card({500, 500}, {1800, 300}, "Rapid Fire", "Increases attack\nspeed by 50%");

    std::array<Button, 4> m_buttons;
    bool exitAnimationStarted;
    Music homeMusic;
};
