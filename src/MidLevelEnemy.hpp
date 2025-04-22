#pragma once
#include "raylib.h"
#include "Enemy.hpp"
#include <string>
#include "TImer.hpp"

class MidLevelEnemy : public Enemy
{
private:
    Vector2 currentDirection;
    float turnSpeed;
    const std::string textureName;
    float bulletSpeed;
    Cooldown fireCooldown; 
    float fireRange = 1.85f;

public:
    MidLevelEnemy(std::string enemyID, EnemyType type, Vector2 spawnPos, Vector2 playerPos);
    void update() override;
    void draw() override;
};