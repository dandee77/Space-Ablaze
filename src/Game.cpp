#include "Game.hpp"
#include <cmath>
#include <iostream>
#include "Animator.hpp"


Game::Game() 
{}

void Game::onSwitch()
{
    worldTileSize = 500.0f; 
    playerEntity = Player();
    playerEntity.init();
    camera = {0};
    camera.target = { playerEntity.getPosition().x + playerEntity.getSize().x/2, playerEntity.getPosition().y + playerEntity.getSize().y/2 };
    camera.offset = {GAME_SCREEN_WIDTH/2.0f, GAME_SCREEN_HEIGHT/2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 15.0f;

    backgroundLayers = {
        {ResourceManager::GetInstance().GetTexture("background1"), 0.2f, worldTileSize}, 
        {ResourceManager::GetInstance().GetTexture("background2"), 0.4f, worldTileSize}, 
        {ResourceManager::GetInstance().GetTexture("background3"), 0.7f, worldTileSize},
        {ResourceManager::GetInstance().GetTexture("background4"), 1.0f, worldTileSize} 
    };
}

std::string Game::update()
{
    if (IsKeyPressed(KEY_ENTER)) return "MainMenu";

    camera.target = {playerEntity.getPosition().x + (playerEntity.getSize().x/2), playerEntity.getPosition().y + (playerEntity.getSize().y/2)};

    playerEntity.update();
    
    return "Game";
}

void Game::DrawLayer(const BackgroundLayer& layer, const Vector2& viewCenter) 
{
    Vector2 parallaxOffset = {
        viewCenter.x * (1.0f - layer.parallaxFactor),
        viewCenter.y * (1.0f - layer.parallaxFactor)
    };

    int startX = static_cast<int>(std::floor((viewCenter.x - parallaxOffset.x) / layer.size));
    int startY = static_cast<int>(std::floor((viewCenter.y - parallaxOffset.y) / layer.size));

    for (int y = -1; y <= 1; y++) { 
        for (int x = -1; x <= 1; x++) {
            Vector2 tilePos = {
                (startX + x) * layer.size + parallaxOffset.x,
                (startY + y) * layer.size + parallaxOffset.y
            };

            DrawTextureV(layer.texture, tilePos, WHITE);
        }
    }
}


void Game::DrawParallaxBackground()
{
    Vector2 viewCenter = camera.target;
    
    for (const auto& layer : backgroundLayers) {
        DrawLayer(layer, viewCenter);
    }
}

void Game::draw()
{
    BeginMode2D(camera);

        DrawParallaxBackground();
        playerEntity.draw();

        Animator::GetInstance().Update();
        Animator::GetInstance().Draw(); 

        std::cout << "Player Position: (" << playerEntity.getPosition().x << ", " << playerEntity.getPosition().y << ")" << std::endl;

    EndMode2D();
}

void Game::onExit()
{

}