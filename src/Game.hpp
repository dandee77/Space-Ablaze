#pragma once
#include "Scene.hpp"
#include "raylib.h"
#include <string>
#include "ResourceManager.hpp"
#include <vector>
#include "Player.hpp"
#include "BulletManager.hpp"
#include "EnemyManager.hpp"

class Game : public Scene
{
public:
    Game();
    std::string update() override;
    void draw() override;
    void onSwitch() override;
    void onExit() override;
    ~Game() override = default;

private:
    Camera2D camera;
        
    struct BackgroundLayer {
        Texture2D texture;
        float parallaxFactor; // 0.0 (stationary) to 1.0 (moves with player)
        float size;
    };
    
    std::vector<BackgroundLayer> backgroundLayers;
    float worldTileSize;
    
    void DrawParallaxBackground();
    void DrawLayer(const BackgroundLayer& layer, const Vector2& viewCenter);

    Player playerEntity;

    BulletManager bulletManager;
    Texture2D playerBulletTexture; 
    Texture2D enemyBulletTexture;
};