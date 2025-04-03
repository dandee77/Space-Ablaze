#include "Game.hpp"
#include <cmath>
#include <iostream>

Game::Game() 
    : player(Rectangle{0, 0, 5, 5}) // Start at center
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
        {ResourceManager::GetInstance().GetTexture("background1"), 0.2f, worldTileSize}, // Foreground (moves with player)
        {ResourceManager::GetInstance().GetTexture("background2"), 0.4f, worldTileSize}, 
        {ResourceManager::GetInstance().GetTexture("background3"), 0.7f, worldTileSize},
        {ResourceManager::GetInstance().GetTexture("background4"), 1.0f, worldTileSize}  // Background (moves slowest)
    };

    for (auto& layer : backgroundLayers) {
        SetTextureFilter(layer.texture, TEXTURE_FILTER_BILINEAR);
        SetTextureWrap(layer.texture, TEXTURE_WRAP_REPEAT);
    }
}

std::string Game::update()
{
    if (IsKeyPressed(KEY_ENTER)) return "MainMenu";

    // Player movement
    if (IsKeyDown(KEY_W)) player.y -= 2.0f;
    if (IsKeyDown(KEY_S)) player.y += 2.0f;
    if (IsKeyDown(KEY_A)) player.x -= 2.0f;
    if (IsKeyDown(KEY_D)) player.x += 2.0f;

    // Update camera to follow player
    camera.target = {player.x + player.width/2, player.y + player.height/2};
    
    return "Game";
}

void Game::DrawLayer(const BackgroundLayer& layer, const Vector2& viewCenter)
{
    // Calculate parallax-adjusted position (like in reference code)
    Vector2 parallaxOffset = {
        viewCenter.x * (1.0f - layer.parallaxFactor),
        viewCenter.y * (1.0f - layer.parallaxFactor)
    };

    // Calculate tile positions (adapted from reference code)
    for (int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
            // Calculate tile grid position
            int tileX = x + static_cast<int>((viewCenter.x - parallaxOffset.x) / layer.size);
            int tileY = y + static_cast<int>((viewCenter.y - parallaxOffset.y) / layer.size);
            
            // Calculate final render position
            Vector2 tilePos = {
                tileX * layer.size + parallaxOffset.x,
                tileY * layer.size + parallaxOffset.y
            };
            
            DrawTextureV(layer.texture, tilePos, WHITE);
        }
    }
}

void Game::DrawParallaxBackground()
{
    Vector2 viewCenter = camera.target;
    
    // Draw from background to foreground
    for (const auto& layer : backgroundLayers) {
        DrawLayer(layer, viewCenter);
    }
}

void Game::draw()
{
    BeginMode2D(camera);
    {
        DrawParallaxBackground();
        DrawRectangleRec(player, BLUE);
        
        // Debug info
        DrawText(TextFormat("Pos: (%.1f, %.1f)", player.x, player.y), 
                -100, -50, 20, WHITE);
    }
    EndMode2D();
}

void Game::onExit()
{
    // Cleanup handled by ResourceManager
}