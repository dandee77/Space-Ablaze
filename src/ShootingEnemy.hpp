#pragma once
#include "raylib.h"
#include "Entity.hpp"
#include "Timer.hpp"
#include <cmath>

class ShootingEnemy : public Entity
{
public:
    ShootingEnemy(Vector2 spawnPos);

    bool update() override;
    void draw(Texture2D enemyTexture) override;
    void getPlayerPosition(Vector2 playerPos);

    Vector2 getPosition() const override { return position; }

    // Add getters/setters for randomness support
    void setSpeed(float s) { speed = s; }
    void setTurnSpeed(float s) { turnSpeed = s; }
    void setFireRange(float r) { fireRange = r; }
    void setBulletSpeed(float s) { bulletSpeed = s; }
    float getBulletSpeed() const { return bulletSpeed; }
    Vector2 getViewDirection() const { return viewDirection; }
    Cooldown& getCooldown() { return fireCooldown; }


private:
    Vector2 position;
    Vector2 viewDirection;
    Vector2 playerPosition;
    float bulletSpeed; 

    float speed;
    float turnSpeed;
    float fireRange = 1.85f;

    float spriteSize = 14.0f;

    Cooldown fireCooldown;
};
