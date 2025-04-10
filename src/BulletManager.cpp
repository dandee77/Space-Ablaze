#include "BulletManager.hpp"
#include "raymath.h"


void BulletManager::update(float deltaTime, Vector2 playerPos) {
    for (int i = 0; i < bullets.size(); i++) {
        float dist = Vector2Distance(bullets[i].getPosition(), playerPos);
        if (dist > BULLET_DESPAWN_DISTANCE) {
            bullets.erase(bullets.begin() + i);
            i--;
            continue;
        }

        bullets[i].update(deltaTime);
    }
}

#include <iostream>

void BulletManager::draw(const Texture2D& playerBulletTexture, const Texture2D& enemyBulletTexture) const {
    for (auto& bullet : bullets) {
        if (bullet.isEnemyBullet()) 
            bullet.draw(enemyBulletTexture);
        else bullet.draw(playerBulletTexture);
    }
}

void BulletManager::addBullet(const Bullet& bullet) {
    bullets.push_back(bullet);
}

void BulletManager::clear() {
    bullets.clear();
}
