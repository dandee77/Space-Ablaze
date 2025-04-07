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

void BulletManager::draw(Texture2D bulletTexture) {
    for (auto& bullet : bullets) {
        bullet.draw(bulletTexture);

        std::cout << "Number of bullets: " << bullets.size() << std::endl;
    }
}

void BulletManager::addBullet(const Bullet& bullet) {
    bullets.push_back(bullet);
}

void BulletManager::clear() {
    bullets.clear();
}
