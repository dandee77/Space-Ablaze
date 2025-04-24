#include "BulletManager.hpp"
#include "raymath.h"


void BulletManager::update(Vector2 position) {
    for (int i = 0; i < bullets.size(); i++) {
        float dist = Vector2Distance(bullets[i].getPosition(), position);
        if (dist > BULLET_DESPAWN_DISTANCE) {
            bullets.erase(bullets.begin() + i);
            i--;
            continue;
        }

        bullets[i].update();
    }
}


void BulletManager::draw(const Texture2D& playerBulletTexture, const Texture2D& enemyBulletTexture) const {
    for (auto& bullet : bullets) {
        if (bullet.isEnemyBullet()) 
            bullet.draw(enemyBulletTexture);
        else bullet.draw(playerBulletTexture);
    }
}


void BulletManager::addBullet(Bullet bullet) {
    bullets.push_back(bullet);
}


void BulletManager::clear() {
    bullets.clear();
}
