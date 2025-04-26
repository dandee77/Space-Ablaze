#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include "BulletManager.hpp"
#include "Timer.hpp"
#include "raylib.h"
#include "Enemy.hpp"
#include <string>
#include "Animator.hpp"


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
    std::unordered_map<std::string, std::unique_ptr<Enemy>>& getEnemies() { return enemies; };
    void removeEnemy(const std::string& id);
    void reset();
private:

    EnemyManager() = default;
    void spawnEnemy(Vector2 playerPos);

    int enemyCounter = 0; 
    std::unordered_map<std::string, std::unique_ptr<Enemy>> enemies; 
    Cooldown enemySpawnCooldown{0.5f}; 
    const int maxEnemies = 10'000;
};
