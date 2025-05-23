#pragma once

#include "raylib.h"
#include "Enemy.hpp"
#include <string>

class LowLevelEnemy : public Enemy
{
private:
    Vector2 currentDirection;
    float turnSpeed;
    const std::string textureName;
    int randomEnemyTextureIdx;

public:
    LowLevelEnemy(std::string enemyID, EnemyType type, Vector2 spawnPos, Vector2 playerPos);
    void update() override;
    void draw() override;
    void destruct() override;
};