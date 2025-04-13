#include "LowLevelEnemy.hpp"
#include "raymath.h"


LowLevelEnemy::LowLevelEnemy(EnemyType type, Vector2 spawnPos, Vector2 playerPos) 
    : Enemy(type, spawnPos, playerPos)
{
    spriteSize = 14.0f;
    origin = {spriteSize / 2.0f, spriteSize / 2.0f};
    speed = GetRandomValue(20, 50);
    turnSpeed = 5;
    currentDirection = {0, 0};
}

void LowLevelEnemy::update()
{
    Vector2 desiredDirection = Vector2Subtract(playerPosition, position);
    float distance = Vector2Length(desiredDirection);
    
    if (distance > 0.0f) 
    {
        // Smoothly rotate towards the player
        desiredDirection = Vector2Normalize(desiredDirection);
        currentDirection = Vector2Lerp(currentDirection, desiredDirection, turnSpeed * GetFrameTime());
        currentDirection = Vector2Normalize(currentDirection);
        
        // Move towards player with smooth movement
        position = Vector2MoveTowards(position, playerPosition, speed * GetFrameTime());
        
        // Or use this alternative if you want more control:
        // position = Vector2Add(position, Vector2Scale(currentDirection, speed * GetFrameTime()));
        
        viewDirection = currentDirection;
    }
}


void LowLevelEnemy::draw(const Texture2D& enemyTexture)
{
    Rectangle source = {0, 0, (float)enemyTexture.width, (float)enemyTexture.height};
    rect = {position.x, position.y, spriteSize, spriteSize};
    float rotation = atan2f(viewDirection.y, viewDirection.x) * RAD2DEG;
    rotation += 90.0f;
    
    DrawTexturePro(enemyTexture, source, rect, origin, rotation, WHITE);
}

 