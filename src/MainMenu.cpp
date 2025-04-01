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
    : m_buttons{Button(Rectangle{500, 500, 700, 200}, "Play", 100, BUTTON_LEFT_SIDE), 
        Button(Rectangle{500, 800, 700, 200}, "Settings", 100, BUTTON_LEFT_SIDE), 
        Button(Rectangle{500, 1100, 700,200}, "Credits", 100, BUTTON_LEFT_SIDE),
        Button(Rectangle{500, 1400, 700,200}, "Exit", 100, BUTTON_LEFT_SIDE)} {}


static void onExitButtonClick()
{
    exit(0);
}


std::string MainMenu::update()
{
    // ? Update the buttons
    for (int idx = 0; idx < m_buttons.size(); ++idx)
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
                    onExitButtonClick();
                default:
                    break;
            }
        }
    }
    return "MainMenu";
}


void MainMenu::draw()
{
    // ? Draw the background texture
    Animator::GetInstance().Update();
    Animator::GetInstance().Draw(Rectangle{0, 0, 3840, 2160}, false, WHITE);

    // ? Draw the buttons
    for (auto& button : m_buttons) button.draw();
}

void MainMenu::onSwitch()
{
    Texture2D background_texture = ResourceManager::GetInstance().GetTexture("main_menu_background");

    int frameWidth = background_texture.width / 5;
    int frameHeight = background_texture.height / 2;
    
    auto bgAnim = std::make_shared<Animation>(
        background_texture, 
        frameWidth,
        frameHeight,
        0.15f,  
        true
    );
    
    Animator::GetInstance().AddAnimation("bg", bgAnim);
    Animator::GetInstance().Play("bg");
}

void MainMenu::onExit()
{
    Animator::GetInstance().StopAll();
}

