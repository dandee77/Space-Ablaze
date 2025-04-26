#pragma once
#include "Entity.hpp"
#include "raylib.h"
#include "Timer.hpp"
#include "BulletManager.hpp"


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
    void takeDamage(float timeStateEntered);

private:

    Camera2D camera;
    bool playerAccelerating;
    PlayerState playerState;
    Cooldown playerAttackCooldown;
    bool autoShoot;
    float timeStateEntered;
};