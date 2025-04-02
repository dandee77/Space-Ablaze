#include "MainMenu.hpp"
#include "ResourceManager.hpp"


enum MainMenuButtons
{
    PLAY = 0,
    SETTINGS,
    CREDITS,
    QUIT
};


MainMenu::MainMenu()
    : m_buttons{Button(Rectangle{500, 500, 700, 200}, "> Play", 100, BUTTON_LEFT_SIDE), 
        Button(Rectangle{500, 800, 700, 200}, "> Settings", 100, BUTTON_LEFT_SIDE), 
        Button(Rectangle{500, 1100, 700,200}, "> Credits", 100, BUTTON_LEFT_SIDE),
        Button(Rectangle{500, 1400, 700,200}, "> Exit", 100, BUTTON_LEFT_SIDE)},
        exitAnimationStarted(false) {}


#include <iostream>

std::string MainMenu::update()
{
    // ? Update the buttons
    for (int idx = 0; idx < m_buttons.size() && !exitAnimationStarted; ++idx)
    {
        if (m_buttons[idx].isClicked())
        {
            switch (idx)
            {
                case PLAY:
                    return "Game";
                case SETTINGS:
                    return "Settings";
                case CREDITS:
                    return "Credits";
                case QUIT:
                {
                    Texture2D exit_texture = ResourceManager::GetInstance().GetTexture("exit_animation");
                    Animator::GetInstance().AddAnimation("exit", std::make_shared<Animation>(
                        exit_texture,
                        exit_texture.width / 4,
                        exit_texture.height / 7,
                        0.01f,
                        false,
                        Rectangle{0, 0, GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT},
                        false,
                        WHITE
                    ));
                    Animator::GetInstance().Stop("background");
                    Animator::GetInstance().Play("exit");
                    exitAnimationStarted = true;
                    break;
                }
                default:
                    break;
            }
        }
    }


    if (exitAnimationStarted && Animator::GetInstance().IsCompleted("exit")) exit(0);
    return "MainMenu";
}


void MainMenu::draw()
{
    // ? Draw the background texture
    Animator::GetInstance().Update();
    Animator::GetInstance().Draw();

    // ? Draw the buttons
    if (!exitAnimationStarted)  for (auto& button : m_buttons) button.draw();
}

void MainMenu::onSwitch()
{   
    // ? Load the background animation texture
    Texture2D backgound_texture = ResourceManager::GetInstance().GetTexture("main_menu_background");
    int backgroundFrameWidth = backgound_texture.width / 5;
    int backgroundFrameHeight = backgound_texture.height / 2;
    auto exitAnim = std::make_shared<Animation>(
        backgound_texture, // ? Texture2D sprite sheet
        backgroundFrameWidth, // ? Frame width
        backgroundFrameHeight, // ? Frame height
        0.1f, // ? Frame duration
        true, // ? Looping
        Rectangle{0, 0, GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT}, // ? Destination rectangle
        false, // ? Flip X
        WHITE // ? Tint color
    );
    Animator::GetInstance().AddAnimation("background", exitAnim);
    Animator::GetInstance().Play("background");
}

void MainMenu::onExit()
{
    Animator::GetInstance().StopAll();
}

