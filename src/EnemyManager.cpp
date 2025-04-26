#include "EnemyManager.hpp"
#include "ResourceManager.hpp"
#include "raymath.h"
#include "LowLevelEnemy.hpp"
#include "MidLevelEnemy.hpp"
#include <iostream>


#define ENEMY_MAX_DISTANCE 1000.0f
#define ENEMY_MAX_RANGE 100.0f

void EnemyManager::update(Vector2 playerPos)
{

    // if (enemies.size() < maxEnemies && !enemySpawnCooldown.isOnCooldown())
    // {
    //     // ? could spawn 1 to 3 enemies at once
    //     int spawns = 1 + GetRandomValue(0, 2); 
    //     for (int i = 0; i < spawns; i++)
    //         spawnEnemy(playerPos);
    //     enemySpawnCooldown.startCooldown();
    // }

    // for (int i = 0; i < enemies.size(); i++)
    // {
    //     float dist = Vector2Distance(enemies[i]->getPosition(), playerPos);
    //     // ! deletes anemies that are out of range
    //     // if (dist > ENEMY_MAX_DISTANCE)
    //     // {
    //     //     enemies.erase(enemies.begin() + i);
    //     //     i--;
    //     //     continue;
    //     // }
    //     enemies[i]->getPlayerPositionForEnemy(playerPos); 
    //     enemies[i]->update(); 

    //     // if (enemies[i].update() && dist <= ENEMY_MAX_RANGE) // make sure the enemy is in player's fov
    //     // {
    //     //     Bullet b(enemies[i].getPosition(), enemies[i].getViewDirection(), true);
    //     //     b.setBulletSpeed(enemies[i].getBulletSpeed());
    //     //     BulletManager::GetInstance().addBullet(b);
    //     // }
    // }

    if (enemies.size() < maxEnemies && !enemySpawnCooldown.isOnCooldown()) {
        int spawns = 1 + GetRandomValue(0, 2); 
        for (int i = 0; i < spawns; i++)
            spawnEnemy(playerPos);
        enemySpawnCooldown.startCooldown();
    }

    // std::vector<std::string> toRemove;
    for (auto& [id, enemy] : enemies) {
        float dist = Vector2Distance(enemy->getPosition(), playerPos);
        enemy->getPlayerPositionForEnemy(playerPos);
        enemy->update();
        
        // Add any other removal conditions here
        // if (shouldRemove) toRemove.push_back(id);
    }

    // for (auto& id : toRemove) {
    //     enemies.erase(id);
    // }

    // std::cout << "Enemy count: " << enemies.size() << std::endl;
}


void EnemyManager::draw()
{
    for (auto& [id, enemy] : enemies) {
        enemy->draw(); 
    }
}


void EnemyManager::spawnEnemy(Vector2 playerPos)
{
    Vector2 offset = {150.0f, 0.0f};
    float angle = DEG2RAD * (float)GetRandomValue(0, 359);
    offset = Vector2Rotate(offset, angle);
    Vector2 spawnPos = Vector2Add(playerPos, offset);
    
    std::string enemyID = "enemy_" + std::to_string(enemyCounter++);
    
    std::unique_ptr<Enemy> enemy = nullptr;
    int rand = GetRandomValue(0, 5);
    if (rand == 5) enemy = std::make_unique<MidLevelEnemy>(enemyID, MID_LEVEL_ENEMY, spawnPos, playerPos);
    else enemy = std::make_unique<LowLevelEnemy>(enemyID, LOW_LEVEL_ENEMY, spawnPos, playerPos);
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

    // enemies.erase(enemies.begin() + index);  
}

void EnemyManager::reset()
{
    enemies.clear();
    enemyCounter = 0;
}
