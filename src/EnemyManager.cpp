#include "EnemyManager.hpp"
#include "raymath.h"
#include <iostream>

#define ENEMY_MAX_DISTANCE 1000.0f

void EnemyManager::update(float deltaTime, Vector2 playerPos, BulletManager& bulletManager)
{

    if (enemies.size() < maxEnemies)
    {
        spawnCooldown -= deltaTime;
        spawnCooldown = GetRandomValue(1, 6); 

        int spawns = 1 + (GetRandomValue(0, 2) == 0 ? 2 : 0); 

        for (int i = 0; i < spawns; ++i)
            spawnEnemy(playerPos);
        
    }

    for (int i = 0; i < enemies.size(); i++)
    {
        float dist = Vector2Distance(enemies[i].getPosition(), playerPos);
        if (dist > ENEMY_MAX_DISTANCE)
        {
            enemies.erase(enemies.begin() + i);
            i--;
            continue;
        }

        enemies[i].getPlayerPosition(playerPos); 
        // todo: add condition if the enemy is in the player's fov
        // todo: check if enemy is on upper side or sideways
        if (enemies[i].update() && dist <= 100) // make sure the enemy is in player's fov
        {
            Bullet b(enemies[i].getPosition(), enemies[i].getViewDirection(), true);
            b.setBulletSpeed(enemies[i].getBulletSpeed());
            bulletManager.addBullet(b);
        }
    }

    std::cout << "Enemy count: " << enemies.size() << std::endl; // Debugging line

}


void EnemyManager::draw(Texture2D enemyTexture)
{
    for (auto& enemy : enemies)
    {
        enemy.draw(enemyTexture);
    }
}


void EnemyManager::spawnEnemy(Vector2 playerPos)
{
    Vector2 offset = {100.0f, 0.0f};

    float angle = DEG2RAD * (float)GetRandomValue(0, 359);
    offset = Vector2Rotate(offset, angle);

    Vector2 spawnPos = Vector2Add(playerPos, offset);

    ShootingEnemy enemy(spawnPos);
  
    enemy.setSpeed(GetRandomValue(25, 75)); 
    enemy.setTurnSpeed(1.5f + GetRandomValue(0, 1000) / 1000.0f); 
    enemy.getCooldown().updateCooldownDuration(0.5f + GetRandomValue(0, 750) / 1000.0f); 
    // enemy.getCooldown().updateCooldownDuration(0.01f);
    enemy.setBulletSpeed(GetRandomValue(150, 200)); 
    enemies.push_back(enemy);
}
