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
//todo: make the rand to be a class var so that it will be used when the enemy dies
//TODO: ADD OPTION FOR THE ANIMATION TO BE ON OR OFF IN THE SETTINGS

LowLevelEnemy::LowLevelEnemy(std::string enemyID, EnemyType type, Vector2 spawnPos, Vector2 playerPos) 
    : Enemy(enemyID, type, spawnPos, playerPos), textureName(EnemyTextureNames[GetRandomValue(0, 2)])
{
    health = 30;  
    spriteSize = 15.0f;
    origin = {spriteSize / 2.0f, spriteSize / 2.0f};
    speed = GetRandomValue(15, 30);
    turnSpeed = 5;
    currentDirection = {0, 0};
    rect = {position.x, position.y, spriteSize, spriteSize};
    hitbox = {position.x, position.y, spriteSize, spriteSize};
    
    knockbackVelocity = {0, 0};
    knockbackDecay = 0.9f;
    knockbackCooldown = Cooldown(ENEMY_KNOCKBACK_COOLDOWN); 
    
    separationRadius = 7.5f;
    separationStrength = 10.0f; 

    //! INEFFICIENT WAY TO PASS TEXTURES, BUT IT WORKS FOR NOW
    randomEnemyTextureIdx = GetRandomValue(0, 2);
    switch (randomEnemyTextureIdx) 
    {
    case 0:
        Animator::GetInstance().AddAnimation(enemyID, std::make_shared<Animation>(
            ResourceManager::GetInstance().GetTextureRef("low_level_enemy1"),
            ResourceManager::GetInstance().GetTexture("low_level_enemy1").width / 10,
            ResourceManager::GetInstance().GetTexture("low_level_enemy1").height,
            0.1f,
            true,
            rect,
            false,
            WHITE
        ));
        break;
    case 1:
        Animator::GetInstance().AddAnimation(enemyID, std::make_shared<Animation>(
            ResourceManager::GetInstance().GetTextureRef("low_level_enemy2"),
            ResourceManager::GetInstance().GetTexture("low_level_enemy2").width / 5,
            ResourceManager::GetInstance().GetTexture("low_level_enemy2").height,
            0.1f,
            true,
            rect,
            false,
            WHITE
        ));
        break;
    case 2:
        Animator::GetInstance().AddAnimation(enemyID, std::make_shared<Animation>(
            ResourceManager::GetInstance().GetTextureRef("low_level_enemy3"),
            ResourceManager::GetInstance().GetTexture("low_level_enemy3").width / 5,
            ResourceManager::GetInstance().GetTexture("low_level_enemy3").height,
            0.1f,
            true,
            rect,
            false,
            WHITE
        ));
        break;
    default:
        TraceLog(LOG_ERROR, "invalid enemy texture id: %d", rand);
        break;
    }

    Animator::GetInstance().SetOrigin(enemyID, origin);
    Animator::GetInstance().Play(enemyID);
    hitbox = {position.x, position.y, spriteSize / 3.5f, spriteSize / 3.5f};
}

void LowLevelEnemy::update()
{
    if (Vector2Length(knockbackVelocity) > 0.1f) {
        position = Vector2Add(position, Vector2Scale(knockbackVelocity, GetFrameTime()));
        knockbackVelocity = Vector2Scale(knockbackVelocity, knockbackDecay);
    }
    
    Vector2 separationForce = calculateSeparation(nearbyEnemyPositions);
    
    Vector2 desiredDirection = Vector2Subtract(playerPosition, position);
    float distance = Vector2Length(desiredDirection);
    
    if (distance > 0.0f) 
    {
        desiredDirection = Vector2Normalize(desiredDirection);
        
        Vector2 combinedDirection = Vector2Add(desiredDirection, separationForce);
        combinedDirection = Vector2Normalize(combinedDirection);
        
        currentDirection = Vector2Lerp(currentDirection, combinedDirection, turnSpeed * GetFrameTime());
        currentDirection = Vector2Normalize(currentDirection);
        
        Vector2 moveDirection = Vector2Scale(currentDirection, speed * GetFrameTime());
        position = Vector2Add(position, moveDirection);
        
        viewDirection = currentDirection;
    }

    rotation = atan2f(viewDirection.y, viewDirection.x) * RAD2DEG;
    rotation += 90.0f;
}


void LowLevelEnemy::destruct()
{
    Animator::GetInstance().Remove(enemyID);
    
    switch (randomEnemyTextureIdx)
    {
    case 0:
        Animator::GetInstance().AddAnimation(enemyID, std::make_shared<Animation>(
            ResourceManager::GetInstance().GetTextureRef("low_level_enemy1_destruct"),
            ResourceManager::GetInstance().GetTexture("low_level_enemy1_destruct").width / 8,
            ResourceManager::GetInstance().GetTexture("low_level_enemy1_destruct").height,
            0.1f,
            false,
            rect,
            false,
            WHITE
        ));
        break;
    case 1:
        Animator::GetInstance().AddAnimation(enemyID, std::make_shared<Animation>(
            ResourceManager::GetInstance().GetTextureRef("low_level_enemy2_destruct"),
            ResourceManager::GetInstance().GetTexture("low_level_enemy2_destruct").width / 8,
            ResourceManager::GetInstance().GetTexture("low_level_enemy2_destruct").height,
            0.1f,
            false,
            rect,
            false,
            WHITE
        ));
        break;
    case 2:
        Animator::GetInstance().AddAnimation(enemyID, std::make_shared<Animation>(
            ResourceManager::GetInstance().GetTextureRef("low_level_enemy3_destruct"),
            ResourceManager::GetInstance().GetTexture("low_level_enemy3_destruct").width / 7,
            ResourceManager::GetInstance().GetTexture("low_level_enemy3_destruct").height,
            0.1f,
            false,
            rect,
            false,
            WHITE
        ));
        break;
    default:
        TraceLog(LOG_ERROR, "invalid low level enemy texture id: %d", randomEnemyTextureIdx);
        break;
    }

    Animator::GetInstance().SetOrigin(enemyID, origin);
    Animator::GetInstance().SetPosition(enemyID, position);
    Animator::GetInstance().SetRotation(enemyID, rotation);
    Animator::GetInstance().Play(enemyID);
}


void LowLevelEnemy::draw()
{
    const Texture2D& enemyTexture = ResourceManager::GetInstance().GetTextureRef(textureName);
    
    // Rectangle source = {0, 0, (float)enemyTexture.width, (float)enemyTexture.height};
    rect = {position.x, position.y, spriteSize, spriteSize};
    Vector2 hitboxOrigin = {hitbox.width / 2.0f, hitbox.height / 2.0f};
    hitbox.x = rect.x;
    hitbox.y = rect.y;
    
    Animator::GetInstance().SetPosition(enemyID, position);
    Animator::GetInstance().SetRotation(enemyID, rotation);

    // DrawRectanglePro(hitbox, hitboxOrigin, 0, WHITE);
    // DrawTexturePro(enemyTexture, source, rect, origin, rotation, WHITE);
}