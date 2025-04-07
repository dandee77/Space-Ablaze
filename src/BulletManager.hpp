#pragma once
#include <vector>
#include "Bullet.hpp"
#include "raylib.h"

#define BULLET_DESPAWN_DISTANCE 150.0f

class BulletManager {
public:
    void update(float deltaTime, Vector2 playerPos);
    void draw(Texture2D bulletTexture);

    void addBullet(const Bullet& bullet);

    void clear(); // optional: clears all bullets

private:
    std::vector<Bullet> bullets;
};
