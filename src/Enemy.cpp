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

Vector2 Enemy::calculateSeparation(const std::vector<Vector2>& nearbyEnemyPositions) {
    Vector2 separationForce = {0, 0};
    int count = 0;
    
    for (const Vector2& otherPos : nearbyEnemyPositions) {
        float distance = Vector2Distance(position, otherPos);
        
        if (distance > 0.0f && distance < separationRadius) {
            Vector2 diff = Vector2Subtract(position, otherPos);
            diff = Vector2Normalize(diff);
            
            float weight = 1.0f - (distance / separationRadius);
            diff = Vector2Scale(diff, weight);
            
            separationForce = Vector2Add(separationForce, diff);
            count++;
        }
    }
    
    if (count > 0) {
        separationForce = Vector2Scale(separationForce, 1.0f / count);
        separationForce = Vector2Scale(separationForce, separationStrength);
    }
    
    return separationForce;
}
