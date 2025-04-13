#include "EnemyManager.hpp"
#include "ResourceManager.hpp"
#include "raymath.h"
#include "LowLevelEnemy.hpp"
#include <iostream>

#define ENEMY_MAX_DISTANCE 1000.0f
#define ENEMY_MAX_RANGE 100.0f

void EnemyManager::update(Vector2 playerPos)
{

    if (enemies.size() < maxEnemies && !enemySpawnCooldown.isOnCooldown())
    {
        // ? could spawn 1 to 3 enemies at once
        int spawns = 1 + GetRandomValue(0, 2); 
        for (int i = 0; i < spawns; i++)
            spawnEnemy(playerPos);
        enemySpawnCooldown.startCooldown();
    }

    for (int i = 0; i < enemies.size(); i++)
    {
        float dist = Vector2Distance(enemies[i]->getPosition(), playerPos);
        // ! deletes anemies that are out of range
        // if (dist > ENEMY_MAX_DISTANCE)
        // {
        //     enemies.erase(enemies.begin() + i);
        //     i--;
        //     continue;
        // }
        enemies[i]->getPlayerPositionForEnemy(playerPos); 
        enemies[i]->update(); 
        // if (enemies[i].update() && dist <= ENEMY_MAX_RANGE) // make sure the enemy is in player's fov
        // {
        //     Bullet b(enemies[i].getPosition(), enemies[i].getViewDirection(), true);
        //     b.setBulletSpeed(enemies[i].getBulletSpeed());
        //     BulletManager::GetInstance().addBullet(b);
        // }
    }

    std::cout << "Enemy count: " << enemies.size() << std::endl;
}


void EnemyManager::draw()
{
    for (auto& enemy : enemies)
    {
        switch (enemy->getEnemyType())
        {
        case LOW_LEVEL_ENEMY:
            enemy->draw();
            break;
        }
    }
}


void EnemyManager::spawnEnemy(Vector2 playerPos)
{
    Vector2 offset = {150.0f, 0.0f};
    float angle = DEG2RAD * (float)GetRandomValue(0, 359);
    offset = Vector2Rotate(offset, angle);
    Vector2 spawnPos = Vector2Add(playerPos, offset);
    
    enemies.push_back(std::make_unique<LowLevelEnemy>(LOW_LEVEL_ENEMY, spawnPos, playerPos));

    // ShootingEnemy enemy = ShootingEnemy(spawnPos);
    // enemy.getPlayerPosition(playerPos);
  
    // enemy.setSpeed(GetRandomValue(25, 50)); 
    // enemy.setTurnSpeed(1.5f + GetRandomValue(0, 1000) / 1000.0f); 
    // enemy.getCooldown().updateCooldownDuration(0.5f + GetRandomValue(0, 750) / 1000.0f); 
    // // enemy.getCooldown().updateCooldownDuration(0.01f);
    // enemy.setBulletSpeed(GetRandomValue(150, 200)); 
    // enemies.push_back(enemy);
}
