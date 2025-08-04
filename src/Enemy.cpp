#include "Enemy.hpp"
#include "raymath.h"

void Enemy::takeDamage(int damage) {
    health -= damage;
    if (health < 0) {
        health = 0;
    }
}

void Enemy::applyKnockback(Vector2 direction, float force) {
    knockbackVelocity = Vector2Scale(Vector2Normalize(direction), force);
    knockbackDecay = 0.9f; 
    knockbackCooldown.startCooldown(); 
}

bool Enemy::canReceiveKnockback() const {
    return !knockbackCooldown.isOnCooldown();
}
