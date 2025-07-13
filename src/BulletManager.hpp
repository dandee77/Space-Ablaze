#pragma once
#include <vector>
#include "Bullet.hpp"
#include "raylib.h"

#define BULLET_DESPAWN_DISTANCE 150.0f


class BulletManager {
public:

    static BulletManager& GetInstance() {
        static BulletManager instance;
        return instance;
    }

    void update(Vector2 position);
    void draw(const Texture2D& playerBulletTexture, const Texture2D& enemyBulletTexture) const;
    void addBullet(Bullet bullet);
    void clear();   
    std::vector<Bullet>& getBullets() { return bullets; }
    void removeBullet(int index) { bullets.erase(bullets.begin() + index); }
    void reset() { bullets.clear(); }

private:
    BulletManager() = default;
    std::vector<Bullet> bullets;
};
