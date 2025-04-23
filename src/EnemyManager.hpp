#pragma once
#include <vector>
#include <memory>
#include "BulletManager.hpp"
#include "Timer.hpp"
#include "raylib.h"
#include "Enemy.hpp"
#include <string>


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

    int enemyCounter = 0; // ? would serve as a unique ID for the enemy
    std::vector<std::unique_ptr<Enemy>> enemies;
    Cooldown enemySpawnCooldown{0.5f}; 
    const int maxEnemies = 10'000;
};
