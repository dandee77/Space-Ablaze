#pragma once
#include "Entity.hpp"
#include "raylib.h"
#include "Timer.hpp"


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
    bool update(Camera2D& camera);
    Vector2 getDirection() const { return facing_direction; }

private:
    bool playerAccelerating;
    PlayerState playerState;
    Cooldown playerAttackCooldown;
    Vector2 facing_direction;
    bool autoShoot;
};