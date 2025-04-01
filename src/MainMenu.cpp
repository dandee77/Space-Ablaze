#include "MainMenu.hpp"
#include "ResourceManager.hpp"


MainMenu::MainMenu()
{
}

#include <iostream>

std::string MainMenu::update()
{
    if (IsKeyPressed(KEY_ENTER))
    return "Game"; 
    else return "MainMenu";
}


void MainMenu::draw()
{
    // std::cout << "Drawing Main Menu" << std::endl;
    Animator::GetInstance().Update();
    Animator::GetInstance().Draw(Rectangle{0, 0, 3840, 2160}, false, WHITE);
}

void MainMenu::onSwitch()
{
    Texture2D background_texture = ResourceManager::GetInstance().GetTexture("game_background");

    if (background_texture.id == 0) {
        std::cerr << "ERROR: Background texture not loaded!" << std::endl;
        return;
    }

    int frameWidth = background_texture.width / 3;
    int frameHeight = background_texture.height / 3;
    
    auto bgAnim = std::make_shared<Animation>(
        background_texture, 
        frameWidth,
        frameHeight,
        0.1f,  
        true
    );
    
    Animator::GetInstance().AddAnimation("bg", bgAnim);
    Animator::GetInstance().Play("bg");
}

void MainMenu::onExit()
{
    Animator::GetInstance().StopAll();
}

