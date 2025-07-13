#include "EnemyManager.hpp"
#include "ResourceManager.hpp"
#include "raymath.h"
#include "LowLevelEnemy.hpp"
#include "MidLevelEnemy.hpp"
#include <iostream>


#define ENEMY_MAX_DISTANCE 1000.0f
#define ENEMY_MAX_RANGE 100.0f

void EnemyManager::update(Vector2 playerPos, const GameTimer& gameTimer)
{
    float elapsed = gameTimer.getElapsedTime();

    float spawnRate = 0.5f;
    if (elapsed > 120) spawnRate = 0.45f;     
    if (elapsed > 240) spawnRate = 0.35f;       
    if (elapsed > 360) spawnRate = 0.25f;      
    enemySpawnCooldown.updateCooldownDuration(spawnRate);

    std::cout << "Enemy spawn cooldown: " << enemySpawnCooldown.getCooldownDuration() << std::endl;

    if (enemies.size() < maxEnemies && !enemySpawnCooldown.isOnCooldown()) {
        int spawns = 1 + GetRandomValue(0, 2);
        for (int i = 0; i < spawns; i++) {
            spawnEnemy(playerPos, elapsed);
        }
        enemySpawnCooldown.startCooldown();
    }


    std::vector<std::string> toRemove;

    for (auto& [id, enemy] : enemies) {
        float dist = Vector2Distance(enemy->getPosition(), playerPos);
        enemy->getPlayerPositionForEnemy(playerPos);
        enemy->update();

        if (dist > ENEMY_MAX_DISTANCE) {
            toRemove.push_back(id);
        }
    }

    for (const auto& id : toRemove) {
        removeEnemy(id);
    }

    // std::cout << "Enemy count: " << enemies.size() << std::endl;
}


void EnemyManager::draw()
{
    for (auto& [id, enemy] : enemies) {
        enemy->draw(); 
    }
}


void EnemyManager::spawnEnemy(Vector2 playerPos, float elapsedTime)
{
    Vector2 offset = {150.0f, 0.0f};
    float angle = DEG2RAD * (float)GetRandomValue(0, 359);
    offset = Vector2Rotate(offset, angle);
    Vector2 spawnPos = Vector2Add(playerPos, offset);

    std::string enemyID = "enemy_" + std::to_string(enemyCounter++);
    std::unique_ptr<Enemy> enemy = nullptr;

    // === Difficulty curve ===
    // Use time to influence the spawn chance
    int midLevelChance = 0;
    if (elapsedTime > 120) midLevelChance = 10;  // 10% after 2 mins
    if (elapsedTime > 240) midLevelChance = 25;  // 25% after 4 mins
    if (elapsedTime > 360) midLevelChance = 40;  // 40% after 6 mins

    int roll = GetRandomValue(1, 100);
    if (roll <= midLevelChance) {
        enemy = std::make_unique<MidLevelEnemy>(enemyID, MID_LEVEL_ENEMY, spawnPos, playerPos);
    } else {
        enemy = std::make_unique<LowLevelEnemy>(enemyID, LOW_LEVEL_ENEMY, spawnPos, playerPos);
    }

    enemies[enemyID] = std::move(enemy);

    // std::cout << enemyID << std::endl;
    
    // ShootingEnemy enemy = ShootingEnemy(spawnPos);
    // enemy.getPlayerPosition(playerPos);
  
    // enemy.setSpeed(GetRandomValue(25, 50)); 
    // enemy.setTurnSpeed(1.5f + GetRandomValue(0, 1000) / 1000.0f); 
    // enemy.getCooldown().updateCooldownDuration(0.5f + GetRandomValue(0, 750) / 1000.0f); 
    // // enemy.getCooldown().updateCooldownDuration(0.01f);
    // enemy.setBulletSpeed(GetRandomValue(150, 200)); 
    // enemies.push_back(enemy);
}


void EnemyManager::removeEnemy(const std::string& id)
{
    // Animator::GetInstance().Stop(enemies[index]->getEnemyID());
    enemies[id]->destruct(); 
    enemies.erase(id);
}

void EnemyManager::reset()
{
    enemies.clear();
    enemyCounter = 0;
}
