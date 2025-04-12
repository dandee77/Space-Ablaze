#include "LowLevelEnemy.hpp"
#include "raymath.h"


LowLevelEnemy::LowLevelEnemy(EnemyType type, Vector2 spawnPos, Vector2 playerPos) 
    : Enemy(type, spawnPos, playerPos)
{
    spriteSize = 14.0f;
    origin = {spriteSize / 2.0f, spriteSize / 2.0f};
}


void LowLevelEnemy::update()
{
    viewDirection = Vector2Subtract(playerPosition, position);
    float distance = Vector2Length(viewDirection);
    
    if (distance > 0.0f)
    {
        viewDirection = Vector2Normalize(viewDirection);
        position = Vector2Add(position, Vector2Scale(viewDirection, speed * GetFrameTime()));
    }
}


void LowLevelEnemy::draw(const Texture2D& enemyTexture)
{
    Rectangle source = {0, 0, spriteSize, spriteSize};
    rect = {position.x, position.y, spriteSize, spriteSize};
    float rotation = atan2f(viewDirection.y, viewDirection.x) * RAD2DEG;
    
    DrawTexturePro(enemyTexture, source, rect, origin, rotation, WHITE);
}

