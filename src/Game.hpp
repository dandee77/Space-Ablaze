#pragma once
#include "Scene.hpp"
#include "raylib.h"
#include <string>
#include "ResourceManager.hpp"
#include <vector>
#include "Player.hpp"
#include "EnemyManager.hpp"
#include "Asteroid.hpp"
#include "KillCounter.hpp"
#include "GameTimer.hpp"
#include <array>
#include "Card.hpp"
#include <vector>
#include "Utils.hpp"
#include "DamageIndicatorManager.hpp"
#include "ConfettiManager.hpp"
#include "ExplosionConfettiManager.hpp"
#include "GameOverOverlay.hpp"

#define MAX_SOUND_INSTANCES 10


enum GameState
{
    GAME_RUNNING = 0,
    GAME_AUGMENT_SELECTION,
    GAME_PAUSED,
    GAME_OVER,
};

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
    struct BackgroundLayer {
        Texture2D texture;
        float parallaxFactor; // 0.0 (stationary) to 1.0 (moves with player)
        float size;
    };

    GameState gameState;

    std::vector<BackgroundLayer> backgroundLayers;
    float worldTileSize;
    
    void DrawParallaxBackground();
    void DrawLayer(const BackgroundLayer& layer, const Vector2& viewCenter);

    Music gameMusic;
    Player playerEntity;

    Texture2D playerBulletTexture; 
    Texture2D enemyBulletTexture;

    std::array<Sound, MAX_SOUND_INSTANCES> scoreSounds;

    KillCounter killCounter;
    GameTimer gameTimer;

    std::vector<Card> augmentCards;
};