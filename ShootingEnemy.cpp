#include "ShootingEnemy.hpp"
#include "raymath.h"
#include <iostream>


#define ENEMY_MAX_ACCELERATION 800.0f

ShootingEnemy::ShootingEnemy(Vector2 spawnPos)
    : fireCooldown(1.5f) 
{
    position = spawnPos;
    viewDirection = {1, 0};
}

void ShootingEnemy::getPlayerPosition(Vector2 playerPos)
{
    this->playerPosition = playerPos;
}


bool ShootingEnemy::update()
{
    float deltaTime = GetFrameTime();

    Vector2 toPlayer = Vector2Subtract(playerPosition, position);
    if (Vector2Length(toPlayer) == 0.0f) toPlayer = {1, 0};
    else toPlayer = Vector2Normalize(toPlayer);

    bool shouldShoot = Vector2Length(Vector2Add(toPlayer, viewDirection)) >= fireRange;

    if (shouldShoot && !fireCooldown.isOnCooldown())
    {
        fireCooldown.startCooldown();
    }
    else
    {
        shouldShoot = false;
    }

    Vector2 newDir = {};
    if (Vector2Length(Vector2Add(toPlayer, viewDirection)) <= 0.2f)
    {
        if (GetRandomValue(0, 1))
            newDir = {toPlayer.y, -toPlayer.x};
        else
            newDir = {-toPlayer.y, toPlayer.x};
    }
    else
    {
        newDir = Vector2Add(Vector2Scale(toPlayer, turnSpeed * deltaTime), viewDirection);
    }

    float newLength = Vector2Length(newDir);
    viewDirection = Vector2Normalize(newDir);
    newLength = Clamp(newLength, 0.1f, 3.0f);

    Vector2 desiredVelocity = Vector2Scale(viewDirection, speed);
    velocity = Vector2MoveTowards(velocity, desiredVelocity, ENEMY_MAX_ACCELERATION * deltaTime);
    position = Vector2Add(position, Vector2Scale(velocity, deltaTime));

    return shouldShoot;
}


void ShootingEnemy::draw(Texture2D enemyTexture)
{
    float angle = atan2f(viewDirection.y, viewDirection.x) * RAD2DEG + 90.0f;
    Rectangle source = {0, 0, (float)enemyTexture.width, (float)enemyTexture.height};
    Rectangle dest = {position.x, position.y, spriteSize, spriteSize};
    Vector2 origin = {spriteSize / 2, spriteSize / 2};

    DrawTexturePro(enemyTexture, source, dest, origin, angle, WHITE);
}
