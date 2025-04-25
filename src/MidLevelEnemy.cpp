#include "MidLevelEnemy.hpp"
#include "ResourceManager.hpp"
#include "Animator.hpp"
#include "raymath.h"
#include "BulletManager.hpp"

#define ENEMY_MAX_ACCELERATION 800.0f
#define ENEMY_MAX_RANGE 100.0f


MidLevelEnemy::MidLevelEnemy(std::string enemyID, EnemyType type, Vector2 spawnPos, Vector2 playerPos) 
    : Enemy(enemyID, type, spawnPos, playerPos), textureName("mid_level_enemy"), fireCooldown(1.5f)
{
    spriteSize = 15.0f;
    origin = {spriteSize / 2.0f, spriteSize / 2.0f};
    speed = GetRandomValue(25, 50);
    fireCooldown.updateCooldownDuration(0.5f + GetRandomValue(0, 750) / 1000.0f);
    turnSpeed = 1.5f + GetRandomValue(0, 1000) / 1000.0f;
    currentDirection = {0, 0};
    rect = {position.x, position.y, spriteSize, spriteSize};
    hitbox = {position.x, position.y, spriteSize, spriteSize};
    bulletSpeed = GetRandomValue(120, 150);

    Animator::GetInstance().AddAnimation(enemyID, std::make_shared<Animation>(
        ResourceManager::GetInstance().GetTextureRef(textureName),
        ResourceManager::GetInstance().GetTexture(textureName).width / 12,
        ResourceManager::GetInstance().GetTexture(textureName).height,
        0.1f,
        true,
        rect,
        false,
        WHITE
    ));
    Animator::GetInstance().SetOrigin(enemyID, origin);
    Animator::GetInstance().Play(enemyID);
    hitbox = {position.x, position.y, spriteSize / 3.5f, spriteSize / 3.5f};
}


void MidLevelEnemy::update()
{
    float deltaTime = GetFrameTime();

    Vector2 toPlayer = Vector2Subtract(playerPosition, position);
    if (Vector2Length(toPlayer) == 0.0f) toPlayer = {1, 0};
    else toPlayer = Vector2Normalize(toPlayer);

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

#pragma region PlayerShoot

    bool shouldShoot = Vector2Length(Vector2Add(toPlayer, viewDirection)) >= fireRange;
    float dist = Vector2Distance(position, playerPosition);

    if (shouldShoot && !fireCooldown.isOnCooldown() && dist <= ENEMY_MAX_RANGE)
    {
        fireCooldown.startCooldown();
        Bullet b(position, viewDirection, true);
        b.setBulletSpeed(bulletSpeed);
        BulletManager::GetInstance().addBullet(b);
    }

#pragma endregion

    rotation = atan2f(viewDirection.y, viewDirection.x) * RAD2DEG;
    rotation += 90.0f;
}


void MidLevelEnemy::destruct()
{
    Animator::GetInstance().Remove(enemyID);
    Animator::GetInstance().AddAnimation(enemyID, std::make_shared<Animation>(
        ResourceManager::GetInstance().GetTextureRef("mid_level_enemy_destruct"),
        ResourceManager::GetInstance().GetTexture("mid_level_enemy_destruct").width / 7,
        ResourceManager::GetInstance().GetTexture("mid_level_enemy_destruct").height,
        0.1f,
        false,
        rect,
        false,
        WHITE
    ));
    Animator::GetInstance().SetOrigin(enemyID, origin);
    Animator::GetInstance().SetPosition(enemyID, position);
    Animator::GetInstance().SetRotation(enemyID, rotation);
    Animator::GetInstance().Play(enemyID);

    // automatically deletes the enemy animation after the animation is done
}


void MidLevelEnemy::draw()
{
    // const Texture2D& enemyTexture = ResourceManager::GetInstance().GetTextureRef(textureName);
    rect = {position.x, position.y, spriteSize, spriteSize};
    Vector2 hitboxOrigin = {hitbox.width / 2.0f, hitbox.height / 2.0f};
    hitbox.x = rect.x;
    hitbox.y = rect.y;

    Animator::GetInstance().SetPosition(enemyID, position);
    Animator::GetInstance().SetRotation(enemyID, rotation);

    // DrawRectanglePro(hitbox, hitboxOrigin, 0, WHITE);
}

