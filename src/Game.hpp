#pragma once
#include "Scene.hpp"
#include "raylib.h"
#include <string>
#include "ResourceManager.hpp"
#include <vector>
#include "Player.hpp"
#include "EnemyManager.hpp"
#include "Asteroid.hpp"

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

    std::vector<BackgroundLayer> backgroundLayers;
    float worldTileSize;
    
    void DrawParallaxBackground();
    void DrawLayer(const BackgroundLayer& layer, const Vector2& viewCenter);

    Player playerEntity;

    Texture2D playerBulletTexture; 
    Texture2D enemyBulletTexture;

    int killCount = 0;
    float killCounterTilt = 0.0f;
    float killCounterScale = 1.0f;
    Cooldown killCounterCooldown{0.3f}; 

    void updateKillCounterAnimation() {
        if (killCounterCooldown.isOnCooldown()) {
            float progress = 1.0f - (killCounterCooldown.remainingCooldown() / killCounterCooldown.getCooldownDuration());
            
            killCounterTilt = sinf(progress * PI * 4) * 15.0f * (1.0f - progress);
            
            killCounterScale = 1.0f + sinf(progress * PI) * 0.3f;
        } else {
            killCounterTilt = 0.0f;
            killCounterScale = 1.0f;
        }
    }
    
    void incrementKillCount() {
        killCount++;
        killCounterCooldown.startCooldown();
        updateKillCounterAnimation();
    }

    void drawKillCounter() {
        const int fontSize = 120;
        const float padding = 220.0f;
        
        // Get screen position (not affected by camera)
        Vector2 screenPos = {padding, padding};
        
        std::string killText = "X" + std::to_string(killCount);
        
        Font font = ResourceManager::GetInstance().GetFont("primary_font");
        // Calculate text bounds
        Vector2 textSize = MeasureTextEx(font, killText.c_str(), fontSize, 1.0f);
        
        // Draw with animation effects
        Rectangle textRect = {
            screenPos.x,
            screenPos.y,
            textSize.x,
            textSize.y
        };
        
        // Background rectangle
        DrawRectangleRounded(
            (Rectangle){textRect.x - 10, textRect.y - 5, textRect.width + 20, textRect.height + 10},
            0.5f, 5, Fade(BLACK, 0.7f)
        );
        
        // Draw text with animation
        DrawTextPro(
            font,
            killText.c_str(),
            (Vector2){textRect.x + textRect.width/2, textRect.y + textRect.height/2},
            (Vector2){textRect.width/2, textRect.height/2},
            killCounterTilt,
            fontSize,
            2.0f,
            (Color){255, 50, 50, 255} 
        );
        
        // Optional: Draw outline
        DrawRectangleRoundedLines(
            (Rectangle){textRect.x - 10, textRect.y - 5, textRect.width + 20, textRect.height + 10},
            0.5f, 5, RED);
    }
};