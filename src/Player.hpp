#pragma once
#include "Entity.hpp"
#include "raylib.h"
#include "Timer.hpp"
#include "BulletManager.hpp"
#include <array>

#define MAX_SHOOTING_SOUND_INSTANCES 10

enum PlayerState
{
    PLAYER_DEFAULT = 0,
    PLAYER_STUNNED,
    PLAYER_IFRAME,
    PLAYER_DEAD
};


class Player : public Entity
{
public:

    Player();
    void init();
    void update() override;
    void draw() override;
    Rectangle getHitbox() const { return rect; }
    void takeDamage(float timeStateEntered, Vector2 entityPosition, float entitySize);
    PlayerState getPlayerState() const { return playerState; }
    void addScore(int score) { playerScore += score; }
    int getScore() const { return playerScore; }

    void increasePlayerAttackSpeed();
    void increasePlayerHealth();
    void increasePlayerMovementSpeed();
    void increasePlayerIframeDuration();
    void increasePlayerRotationSpeed();
    void increaseTriggerHappyBurst();
    void increaseScatterShot();
    void increasePiercingRounds();
    
    // Damage system
    int getRandomDamage() const;
    int getMinDamage() const { return minDamage; }
    int getMaxDamage() const { return maxDamage; }


private:
    

    void fireScatterBullets(Vector2 baseDirection);
    
// plyr attrbutes (some are on the entity class)
    float attackSpeed;
    float iframeDuration;
    float baseRotationSpeed;
    float maxRotationSpeed;
    float rotationSpeedMultiplier;

 // Trigger Happy burst fire variables
    int burstCount;
    int currentBurstShot;
    float burstTimer;
    float burstDelay;
    bool isBurstActive;

 // Scatter Shot variables
    int scatterCount;

// Piercing Rounds variables
    int bulletPiercePower;

    // Damage system
    int minDamage;
    int maxDamage;

    int playerScore;
    Camera2D camera;
    bool playerAccelerating;
    PlayerState playerState;
    Cooldown playerAttackCooldown;
    bool autoShoot;
    float timeStateEntered;
    std::array<Sound, MAX_SHOOTING_SOUND_INSTANCES> shootingSounds;
};