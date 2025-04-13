#include "LowLevelEnemy.hpp"
#include "ResourceManager.hpp"
#include "raymath.h"


static const std::string EnemyTextureNames[] = 
{
    "low_level_enemy1",
    "low_level_enemy2",
    "low_level_enemy3"
};

LowLevelEnemy::LowLevelEnemy(EnemyType type, Vector2 spawnPos, Vector2 playerPos) 
    : Enemy(type, spawnPos, playerPos), textureName(EnemyTextureNames[GetRandomValue(0, 2)])
{
    spriteSize = 8.0f;
    origin = {spriteSize / 2.0f, spriteSize / 2.0f};
    speed = GetRandomValue(15, 30);
    turnSpeed = 5;
    currentDirection = {0, 0};
}

void LowLevelEnemy::update()
{
    Vector2 desiredDirection = Vector2Subtract(playerPosition, position);
    float distance = Vector2Length(desiredDirection);
    
    if (distance > 0.0f) 
    {
        desiredDirection = Vector2Normalize(desiredDirection);
        currentDirection = Vector2Lerp(currentDirection, desiredDirection, turnSpeed * GetFrameTime());
        currentDirection = Vector2Normalize(currentDirection);
        position = Vector2MoveTowards(position, playerPosition, speed * GetFrameTime());
        viewDirection = currentDirection;
    }
}


void LowLevelEnemy::draw()
{
    const Texture2D& enemyTexture = ResourceManager::GetInstance().GetTextureRef(textureName);
    
    Rectangle source = {0, 0, (float)enemyTexture.width, (float)enemyTexture.height};
    rect = {position.x, position.y, spriteSize, spriteSize};
    float rotation = atan2f(viewDirection.y, viewDirection.x) * RAD2DEG;
    rotation += 90.0f;
    
    DrawTexturePro(enemyTexture, source, rect, origin, rotation, WHITE);
}