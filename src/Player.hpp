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

    // void setPlayerAttackSpeed(float attackSpeed);
    // void setPlayerFireRate(int fireRate);
    // void setPlayerMovementSpeed(float movementSpeed);
    // void setPlayerHealth(int health);
    // void setPlayerBulletPierce(int piercePower);
    // void setPlayerIframeDuration(float duration);
    // void setPlayerRotationSpeed(float rotationSpeed);

private:

    int playerScore;
    Camera2D camera;
    bool playerAccelerating;
    PlayerState playerState;
    Cooldown playerAttackCooldown;
    bool autoShoot;
    float timeStateEntered;
    std::array<Sound, MAX_SHOOTING_SOUND_INSTANCES> shootingSounds;
};