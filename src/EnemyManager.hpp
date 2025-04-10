#pragma once
#include <vector>
#include "ShootingEnemy.hpp"
#include "BulletManager.hpp"
#include "Timer.hpp"
#include "raylib.h"

class EnemyManager
{
public:
    static EnemyManager& GetInstance()
    {
        static EnemyManager instance;
        return instance;
    }

    void update(float deltaTime, Vector2 playerPos, BulletManager& bulletManager);
    void draw(Texture2D enemyTexture);

private:
    EnemyManager() = default;
    void spawnEnemy(Vector2 playerPos);

    std::vector<ShootingEnemy> enemies;
    float spawnCooldown = 0.0f;

    const int maxEnemies = 5;
};
