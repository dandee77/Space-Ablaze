#include "Asteroid.hpp"


#define ASTEROID_MAX_DISTANCE 200.0f

int Asteroid::asteroidCounter = 0;

Asteroid::Asteroid(Vector2 playerPos)
    : playerPos(playerPos), asteroidID("")
{
    const Texture2D& texture = ResourceManager::GetInstance().GetTextureRef("asteroid");
    rotation = GetRandomValue(0, 360);
    rotationSpeed = GetRandomValue(ASTEROID_ROT_SPEED_MIN, ASTEROID_ROT_SPEED_MAX);
    active = true;

    switch (GetRandomValue(1, 3))
    {
    case 1:
        size = SMALL_ASTEROID;
        break;
    case 2:
        size = MEDIUM_ASTEROID;
        break;
    case 3:
        size = LARGE_ASTEROID;
        break;
    default:
        TraceLog(LOG_ERROR, "invalid asteroid size");
        exit(0);
        break;
    }
    
    Vector2 offset = {150.0f, 0.0f};
    float angle = DEG2RAD * (float)GetRandomValue(0, 359);
    offset = Vector2Rotate(offset, angle);
    position = Vector2Add(playerPos, offset);

    velocity = playerPos;
    velocity = Vector2Scale(Vector2Normalize(velocity), GetRandomValue(ASTEROID_SPEED_MIN, ASTEROID_SPEED_MAX));
    velocity = Vector2Rotate(velocity, GetRandomValue(-ASTEROID_RANDOM_ANGLE, ASTEROID_RANDOM_ANGLE));

    asteroidID = "asteroid_" + std::to_string(asteroidCounter++);
}


void Asteroid::update()
{
    if (active)
    {
        position = Vector2Add(Vector2Scale(velocity, GetFrameTime()), position);    
        rotation += rotationSpeed * GetFrameTime();

        float dist = Vector2Distance(position, playerPos);
        if (dist > ASTEROID_MAX_DISTANCE) active = false;
    }
}


void Asteroid::draw()
{
    const Texture2D& texture = ResourceManager::GetInstance().GetTextureRef("asteroid");
    DrawTexturePro(texture, (Rectangle){ 0, 0, (float)texture.width, (float)texture.height },
                    (Rectangle){ position.x, position.y, float(10 * size), float(10 * size) },
                    (Vector2){ float((10 * size) / 2), float((10 * size) / 2) }, rotation, WHITE);
}


