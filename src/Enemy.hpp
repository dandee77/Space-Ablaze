#pragma once

#include "raylib.h"
#include "Entity.hpp"
#include "Timer.hpp"
#include <string>
#include <vector>

#define ENEMY_KNOCKBACK_COOLDOWN 0.5f 

enum EnemyType
{
    LOW_LEVEL_ENEMY = 0,
    MID_LEVEL_ENEMY,
    HIGH_LEVEL_ENEMY,
    BOSS_LEVEL_ENEMY
};


class Enemy : public Entity
{
public:

    Enemy(std::string enemyID, EnemyType type, Vector2 spawnPos, Vector2 playerPos)
    {
        this->enemyID = enemyID;
        enemyType = type;
        position = spawnPos;
        playerPosition = playerPos;
        viewDirection = {0, 0};
    };
    // virtual void update() = 0;
    // virtual void draw(const Texture2D& enemyTexture) = 0;
    inline void getPlayerPositionForEnemy(Vector2 playerPos) { this->playerPosition = playerPos; };
    inline EnemyType getEnemyType() const { return enemyType; };
    inline Rectangle getHitbox() const { return hitbox; };
    std::string getEnemyID() const { return enemyID; };
    virtual void destruct() = 0;
    
    void takeDamage(int damage);
    bool isDead() const { return health <= 0; }
    void applyKnockback(Vector2 direction, float force);
    bool canReceiveKnockback() const;

protected:

    std::string enemyID;
    EnemyType enemyType;
    Vector2 viewDirection;
    Vector2 playerPosition;
    Rectangle hitbox;
    float speed;
    float spriteSize;
    

    Vector2 knockbackVelocity;
    float knockbackDecay;
    Cooldown knockbackCooldown;
};