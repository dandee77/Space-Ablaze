#pragma once
#include "raylib.h"

class Bullet
{
public:
    Bullet(Vector2 position, Vector2 direction, bool isEnemy, int piercePower = 1, float knockbackPower = 0.0f);

    void update();        
    void draw(const Texture2D &texture) const;             
    Vector2 getPosition() const { return position; }
    Vector2 getDirection() const { return fireDirection; }
    bool isEnemyBullet() const { return enemyBullet; }
    void setBulletSpeed(float s) { speed = s; }
    int getPiercePower() const { return piercePower; }
    void reducePiercePower() { piercePower--; }
    bool canPierce() const { return piercePower > 0; }
    float getKnockbackPower() const { return knockbackPower; }

private:
    Vector2 position;
    Vector2 fireDirection;
    float speed = 200.0f;
    bool enemyBullet;
    int piercePower;
    float knockbackPower;

    float angle = 0.0f;
};
