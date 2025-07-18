#include "EnemyManager.hpp"
#include "ResourceManager.hpp"
#include "raymath.h"
#include "LowLevelEnemy.hpp"
#include "MidLevelEnemy.hpp"
#include <iostream>


#define ENEMY_MAX_DISTANCE 1000.0f
#define ENEMY_MAX_RANGE 100.0f
#define MID_LEVEL_ENEMY_SPAWN_START_TIME 300.0f 

void EnemyManager::update(Vector2 playerPos, const GameTimer& gameTimer)
{
    float elapsed = gameTimer.getElapsedTime();

    float newCooldown = std::max(0.0f, 1.5f - (elapsed / 300.0f) * 1.25f);
    enemySpawnCooldown.updateCooldownDuration(newCooldown);

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
        
        std::vector<Vector2> nearbyPositions;
        Vector2 currentPos = enemy->getPosition();
        for (const auto& [otherId, otherEnemy] : enemies) {
            if (otherId != id) { 
                Vector2 otherPos = otherEnemy->getPosition();
                float distance = Vector2Distance(currentPos, otherPos);
                if (distance < 50.0f) { 
                    nearbyPositions.push_back(otherPos);
                }
            }
        }
        enemy->setNearbyEnemies(nearbyPositions);
        
        enemy->update();
        if (dist > ENEMY_MAX_DISTANCE) {
            toRemove.push_back(id);
        }
    }

    for (const auto& id : toRemove) {
        removeEnemy(id);
    }

    std::cout << "enemy spawn rate: " << newCooldown << std::endl;

    // print number of enemies
    // std::cout << "Number of enemies: " << enemies.size() << std::endl;
}


void EnemyManager::draw()
{
    for (auto& [id, enemy] : enemies) {
        enemy->draw(); 
    }
}


void EnemyManager::spawnEnemy(Vector2 playerPos, float elapsed)
{
    Vector2 offset = {150.0f, 0.0f};
    float angle = DEG2RAD * (float)GetRandomValue(0, 359);
    offset = Vector2Rotate(offset, angle);
    Vector2 spawnPos = Vector2Add(playerPos, offset);

    std::string enemyID = "enemy_" + std::to_string(enemyCounter++);

    // Mid-level spawn chance: Only after 5 minutes, then scales up
    // Time	       Approx. midChance
    // 0-300s (0-5min)    0%
    // 360s        (6min)	11.8%
    // 420s        (7min)	20.3%
    // 480s        (8min)	26.3%
    // 540s        (9min)	29.6%
    // 600s        (10min)	31.5%
    // 900s        (15min)	38.7%
    // ∞	        40% (asymptotic)
    float midChance = 0.0f;
    if (elapsed >= MID_LEVEL_ENEMY_SPAWN_START_TIME) { //?  only spawn mid-level enemies after 5 minutes
        float adjustedTime = elapsed - MID_LEVEL_ENEMY_SPAWN_START_TIME; //? start scaling from 0 after 5 minutes
        midChance = 40.0f * (1.0f - expf(-adjustedTime / 180.0f));
    }
    float randomValue = (float)GetRandomValue(0, 9999) / 100.0f;
    bool isMidLevel = randomValue < midChance;

    std::unique_ptr<Enemy> enemy;
    if (isMidLevel) {
        enemy = std::make_unique<MidLevelEnemy>(enemyID, MID_LEVEL_ENEMY, spawnPos, playerPos);
    } else {
        enemy = std::make_unique<LowLevelEnemy>(enemyID, LOW_LEVEL_ENEMY, spawnPos, playerPos);
    }

    enemies[enemyID] = std::move(enemy);

    // std::cout << "Elapsed: " << elapsed << "s, midChance: " << midChance << "%, randomValue: " << randomValue << ", isMidLevel: " << (isMidLevel ? "true" : "false") << std::endl;

    // ! Don't work anymore, as enemy class don't have this implementations
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
