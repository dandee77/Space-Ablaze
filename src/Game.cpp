#include "Game.hpp"
#include <cmath>
#include <iostream>

Game::Game() 
    : player(Rectangle{0, 0, 5, 5}) 
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
        {ResourceManager::GetInstance().GetTexture("background1"), 1.0f, worldTileSize},
        {ResourceManager::GetInstance().GetTexture("background2"), 0.7f, worldTileSize}, 
        {ResourceManager::GetInstance().GetTexture("background3"), 0.4f, worldTileSize},
        {ResourceManager::GetInstance().GetTexture("background4"), 0.2f, worldTileSize}  
    };

    for (auto& layer : backgroundLayers) {
        SetTextureFilter(layer.texture, TEXTURE_FILTER_BILINEAR);
    }
}

std::string Game::update()
{
    if (IsKeyPressed(KEY_ENTER)) return "MainMenu";


    if (IsKeyDown(KEY_W)) player.y -= 2.0f;
    if (IsKeyDown(KEY_S)) player.y += 2.0f;
    if (IsKeyDown(KEY_A)) player.x -= 2.0f;
    if (IsKeyDown(KEY_D)) player.x += 2.0f;

    camera.target = {player.x + player.width/2, player.y + player.height/2};
    
    return "Game";
}

void Game::DrawLayer(const BackgroundLayer& layer)
{
    Vector2 parallaxOffset = {
        camera.target.x * (1.0f - layer.parallaxFactor),
        camera.target.y * (1.0f - layer.parallaxFactor)
    };


    int64_t centerX = static_cast<int64_t>(parallaxOffset.x / layer.size);
    int64_t centerY = static_cast<int64_t>(parallaxOffset.y / layer.size);


    for (int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
            Vector2 tilePos = {
                (centerX + x) * layer.size - fmod(parallaxOffset.x, layer.size),
                (centerY + y) * layer.size - fmod(parallaxOffset.y, layer.size)
            };
            DrawTextureV(layer.texture, tilePos, WHITE);
        }
    }
}

void Game::DrawParallaxBackground()
{
    for (const auto& layer : backgroundLayers) {
        DrawLayer(layer);
    }
}

void Game::draw()
{
    BeginMode2D(camera);
    {
        DrawParallaxBackground();
        DrawRectangleRec(player, BLUE);
        
        std::cout << "Player Position: (" << player.x << ", " << player.y << ")" << std::endl;
    }
    EndMode2D();
}

void Game::onExit()
{
    // No special cleanup needed - textures managed by ResourceManager
}