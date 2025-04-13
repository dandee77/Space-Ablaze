#pragma once
#include <vector>
#include <memory>
#include "BulletManager.hpp"
#include "Timer.hpp"
#include "raylib.h"
#include "Enemy.hpp"

class EnemyManager
{
public:
    static EnemyManager& GetInstance()
    {
        static EnemyManager instance;
        return instance;
    }

    void update(Vector2 playerPos);
    void draw();

private:
    EnemyManager() = default;
    void spawnEnemy(Vector2 playerPos);

    std::vector<std::unique_ptr<Enemy>> enemies;
    Cooldown enemySpawnCooldown{1.0f}; 
    const int maxEnemies = 500;
};
