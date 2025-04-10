#include "LoadingScreen.hpp"
#include "ResourceManager.hpp"



LoadingScreen::LoadingScreen()
{
    ResourceManager::GetInstance().LoadTextures(); 
}


std::string LoadingScreen::update()
{
    ResourceManager::GetInstance().ProcessLoadedTextures();
    if (ResourceManager::GetInstance().GetTextureLoadingProgress() >= 100)
        return "MainMenu"; 
    return "LoadingScreen";
}

#include <iostream>
void LoadingScreen::draw()
{
    DrawText(TextFormat("Loading progress: %d%%", 
        ResourceManager::GetInstance().GetTextureLoadingProgress()), 
        1200, 1800, 120, WHITE);
}

void LoadingScreen::onSwitch()
{
}

void LoadingScreen::onExit()
{
}