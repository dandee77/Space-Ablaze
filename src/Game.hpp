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
#include "Button.hpp"

#define MAX_SOUND_INSTANCES 10

enum KillerType
{
    KILLER_ENEMY_BULLET = 0,
    KILLER_ENEMY_COLLISION,
    KILLER_ASTEROID
};

struct DeathInfo
{
    KillerType killerType;
    std::string killerID;
    std::string killerName;
    Texture2D killerTexture;
};

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
    ~Game();
    std::string update() override;
    void draw() override;
    void onSwitch() override;
    void onExit() override;

private:        
    struct BackgroundLayer {
        Texture2D texture;
        float parallaxFactor; // 0.0 (stationary) to 1.0 (moves with player)
        float size;
    };

    void restartGame();

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
    
    DeathInfo deathInfo;
    Button* restartButton;
    Button* mainMenuButton;
};