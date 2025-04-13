#pragma once

#include "raylib.h"
#include "Enemy.hpp"


class LowLevelEnemy : public Enemy
{
private:

    Vector2 currentDirection;
    float turnSpeed;

public:

    LowLevelEnemy(EnemyType type, Vector2 spawnPos, Vector2 playerPos);
    void update() override;
    void draw(const Texture2D& enemyTexture) override;
};