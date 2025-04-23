#include "Bullet.hpp"
#include "raymath.h"

Bullet::Bullet(Vector2 pos, Vector2 direction, bool isEnemy)
    : position(pos), fireDirection(Vector2Normalize(direction)), enemyBullet(isEnemy)
{
    angle = atan2f(fireDirection.y, fireDirection.x) * RAD2DEG;
}

void Bullet::update()
{
    position = Vector2Add(Vector2Scale(fireDirection, speed * GetFrameTime()), position);
}

void Bullet::draw(const Texture2D &texture) const
{
    Vector2 size = isEnemyBullet() ? Vector2{8.0f, 8.0f} : Vector2{8.0f, 5.0f};
    Vector2 drawPos = position;

    DrawTexturePro(
        texture,
        {0, 0, (float)texture.width, (float)texture.height},
        {drawPos.x, drawPos.y, size.x, size.y},
        {size.x / 2.0f, size.y / 2.0f},
        angle,
        WHITE
    );
}