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
    std::cout << "Drawing Main Menu" << std::endl;
    m_animator.Update();
    m_animator.Draw(Rectangle{0, 0, 3840, 2160}, false, WHITE);
}

void MainMenu::onSwitch()
{
    Texture2D background_texture = ResourceManager::GetInstance().GetTexture("game_background");
    auto bgAnim = std::make_shared<Animation>(background_texture, background_texture.width / 3, background_texture.height / 3, 0.1f, true);
    m_animator.AddAnimation("bg", bgAnim);
    m_animator.Play("bg");
}

void MainMenu::onExit()
{
}

