#include "LowLevelEnemy.hpp"
#include "ResourceManager.hpp"
#include "Animator.hpp"
#include "raymath.h"


static const std::string EnemyTextureNames[] = 
{
    "low_level_enemy1",
    "low_level_enemy2",
    "low_level_enemy3"
};

//TODO: ADD OPTION FOR THE ANIMATION TO BE ON OR OFF IN THE SETTINGS

LowLevelEnemy::LowLevelEnemy(std::string enemyID, EnemyType type, Vector2 spawnPos, Vector2 playerPos) 
    : Enemy(enemyID, type, spawnPos, playerPos), textureName(EnemyTextureNames[GetRandomValue(0, 2)])
{
    spriteSize = 15.0f;
    origin = {spriteSize / 2.0f, spriteSize / 2.0f};
    speed = GetRandomValue(15, 30);
    turnSpeed = 5;
    currentDirection = {0, 0};
    rect = {position.x, position.y, spriteSize, spriteSize};


    //! INEFFICIENT WAY TO PASS TEXTURES, BUT IT WORKS FOR NOW
    Animator::GetInstance().AddAnimation(enemyID, std::make_shared<Animation>(
        ResourceManager::GetInstance().GetTextureRef("low_level_enemy4"),
        ResourceManager::GetInstance().GetTexture("low_level_enemy4").width / 5,
        ResourceManager::GetInstance().GetTexture("low_level_enemy4").height,
        0.1f,
        true,
        rect,
        false,
        WHITE
    ));
    Animator::GetInstance().SetOrigin(enemyID, origin);
    Animator::GetInstance().Play(enemyID);
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
    
    // Rectangle source = {0, 0, (float)enemyTexture.width, (float)enemyTexture.height};
    rect = {position.x, position.y, spriteSize, spriteSize};
    float rotation = atan2f(viewDirection.y, viewDirection.x) * RAD2DEG;
    rotation += 90.0f;
    
    Animator::GetInstance().SetPosition(enemyID, position);
    Animator::GetInstance().SetRotation(enemyID, rotation);

    // DrawTexturePro(enemyTexture, source, rect, origin, rotation, WHITE);
}