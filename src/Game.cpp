#include "Game.hpp"
#include <cmath>
#include <iostream>

Game::Game() 
    : player(Rectangle{250, 250, 5, 5}), worldTileSize(500.0f)
{
    camera = {0};
    camera.target = {player.x + player.width/2, player.y + player.height/2};
    camera.offset = {GAME_SCREEN_WIDTH/2.0f, GAME_SCREEN_HEIGHT/2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 15.0f;
}

void Game::onSwitch()
{
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

    if (IsKeyDown(KEY_W)) player.y -= 5.0f;
    if (IsKeyDown(KEY_S)) player.y += 5.0f;
    if (IsKeyDown(KEY_A)) player.x -= 5.0f;
    if (IsKeyDown(KEY_D)) player.x += 5.0f;

    camera.target = {player.x + (player.width/2), player.y + (player.height/2)};
    
    return "Game";
}

void Game::DrawLayer(const BackgroundLayer& layer, const Vector2& viewCenter)
{
    Vector2 parallaxOffset = {
        viewCenter.x * (1.0f - layer.parallaxFactor),
        viewCenter.y * (1.0f - layer.parallaxFactor)
    };

    for (int y = -2; y <= 2; y++) {  // changed to -2 to 2 for more tiles (it's buggin out with -1 to 1)
        for (int x = -2; x <= 2; x++) {
            int tileX = x + static_cast<int>((viewCenter.x - parallaxOffset.x) / layer.size);
            int tileY = y + static_cast<int>((viewCenter.y - parallaxOffset.y) / layer.size);
            
            Vector2 tilePos = {
                tileX * layer.size + parallaxOffset.x,
                tileY * layer.size + parallaxOffset.y
            };
            
            // DrawTextureV(layer.texture, tilePos, WHITE);
            DrawTexturePro(layer.texture, 
                Rectangle{0, 0, (float)layer.texture.width, (float)layer.texture.height}, 
                Rectangle{tilePos.x, tilePos.y, layer.size, layer.size}, 
                Vector2{0, 0}, 0.0f, WHITE);
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
        DrawRectangleRec(player, BLUE);
        
        std::cout << "Player Position: (" << player.x << ", " << player.y << ")" << std::endl;

    EndMode2D();
}

void Game::onExit()
{

}