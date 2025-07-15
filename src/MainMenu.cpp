#include "MainMenu.hpp"
#include "ResourceManager.hpp"

// TODO: ADD PARALLAX BACKGROUND THAT MOVES WITH MOUSE (SEE TEXTURES AT SOON)

enum MainMenuButtons
{
    PLAY = 0,
    SETTINGS,
    CREDITS,
    QUIT
};


MainMenu::MainMenu()
    : m_buttons{Button(Rectangle{250, 1200, 1500, 150}, "> Play", 150, BUTTON_LEFT_SIDE), 
        Button(Rectangle{250, 1370, 1500, 150}, "> Settings", 150, BUTTON_LEFT_SIDE), 
        Button(Rectangle{250, 1540, 1500, 150}, "> Credits", 150, BUTTON_LEFT_SIDE),
        Button(Rectangle{250, 1710, 1500, 150}, "> Exit", 150, BUTTON_LEFT_SIDE)},
        exitAnimationStarted(false) {}


std::string MainMenu::update()
{
    UpdateMusicStream(homeMusic);

    // ? Update the buttons
    for (size_t idx = 0; idx < m_buttons.size() && !exitAnimationStarted; ++idx)
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
                    const Texture2D& exit_texture = ResourceManager::GetInstance().GetTextureRef("exit_animation");
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
    if (!exitAnimationStarted)
    {
        for (auto& button : m_buttons) button.draw();

        // ? Draw the title at the center top
        // ? The "SPACE A" is color SKYBLUE while the "BLAZE" is color RED
        Vector2 text_dimensions = MeasureTextEx(ResourceManager::GetInstance().GetFont("primary_font"), "SPACE", 400, 1.0f);
        float text_x = (GAME_SCREEN_WIDTH - text_dimensions.x) / 2;
        float text_y = 200;
        DrawTextEx(ResourceManager::GetInstance().GetFont("primary_font"), "SPACE  ", (Vector2){ text_x - 80, text_y }, 400, 1.0f, Fade(SKYBLUE, 0.55f));
        text_dimensions = MeasureTextEx(ResourceManager::GetInstance().GetFont("primary_font"), "BLAZE", 400, 1.0f);
        text_x = (GAME_SCREEN_WIDTH - text_dimensions.x) / 2;
        text_y = 480;
        DrawTextEx(ResourceManager::GetInstance().GetFont("primary_font"), "BLAZE", (Vector2){ text_x + 80, text_y }, 400, 1.0f, Fade(RED, 0.55f));

        // ? Draw "Equtiy Quest - 2024" at center bottom
        text_dimensions = MeasureTextEx(ResourceManager::GetInstance().GetFont("secondary_font"), "Equity Quest - 2024", 50, 1.0f);
        text_x = (GAME_SCREEN_WIDTH - text_dimensions.x) / 2;
        text_y = GAME_SCREEN_HEIGHT - 200;
        DrawTextEx(ResourceManager::GetInstance().GetFont("secondary_font"), "Equity Quest - 2024", (Vector2){ text_x, text_y }, 50, 1.0f, Fade(SKYBLUE, 0.55f));
    }
}

void MainMenu::onSwitch()
{   
    // ? Load the background animation texture
    const Texture2D& backgound_texture = ResourceManager::GetInstance().GetTextureRef("main_menu_background");
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

    homeMusic = ResourceManager::GetInstance().GetMusic("home_music");
    PlayMusicStream(homeMusic);
}

void MainMenu::onExit()
{
    Animator::GetInstance().StopAll();
    StopMusicStream(homeMusic);
}

