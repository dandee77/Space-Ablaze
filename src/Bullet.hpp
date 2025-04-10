#pragma once
#include "raylib.h"

class Bullet
{
public:
    Bullet(Vector2 position, Vector2 direction, bool isEnemy = false);

    void update(float deltaTime);        
    void draw(const Texture2D &texture) const;             
    Vector2 getPosition() const { return position; }
    Vector2 getDirection() const { return fireDirection; }
    bool isEnemyBullet() const { return enemyBullet; }
    void setBulletSpeed(float s) { speed = s; }

private:
    Vector2 position;
    Vector2 fireDirection;
    float speed = 200.0f;
    bool enemyBullet;

    float angle = 0.0f;
};
