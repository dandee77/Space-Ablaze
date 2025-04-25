#include "Asteroid.hpp"
#include "Animator.hpp"

#define SMALL_ASTEROID 1.5f
#define MEDIUM_ASTEROID 2.5f
#define LARGE_ASTEROID 3.0f

int Asteroid::asteroidCounter = 0;

Asteroid::Asteroid(Vector2 playerPos)
    : playerPos(playerPos), asteroidID("")
{
    const Texture2D& texture = ResourceManager::GetInstance().GetTextureRef("asteroid");
    rotation = GetRandomValue(0, 360);
    rotationSpeed = GetRandomValue(ASTEROID_ROT_SPEED_MIN, ASTEROID_ROT_SPEED_MAX);
    
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

    origin = { float((10 * size) / 2), float((10 * size) / 2) };

    // float spawnDistance = GetRandomValue(300, 500); 
    float spawnDistance = 150.0f;
    float angle = DEG2RAD * (float)GetRandomValue(0, 359);
    position = {
        playerPos.x + cosf(angle) * spawnDistance,
        playerPos.y + sinf(angle) * spawnDistance
    };

    Vector2 direction = Vector2Subtract(playerPos, position);
    direction = Vector2Normalize(direction);
    
    float speed = GetRandomValue(ASTEROID_SPEED_MIN, ASTEROID_SPEED_MAX);
    velocity = Vector2Scale(direction, speed);
    
    float angleVariation = DEG2RAD * GetRandomValue(-ASTEROID_RANDOM_ANGLE, ASTEROID_RANDOM_ANGLE);
    velocity = Vector2Rotate(velocity, angleVariation);

    asteroidID = "asteroid_" + std::to_string(asteroidCounter++);
}

void Asteroid::update()
{
    position = Vector2Add(Vector2Scale(velocity, GetFrameTime()), position);    
    rotation += rotationSpeed * GetFrameTime();
}


void Asteroid::draw()
{
    Vector2 origin = { (getAsteroidSize()) / 2, (getAsteroidSize()) / 2 };
    const Texture2D& texture = ResourceManager::GetInstance().GetTextureRef("asteroid");
    DrawTexturePro(texture, (Rectangle){ 0, 0, (float)texture.width, (float)texture.height },
                (Rectangle){ position.x, position.y, float(10 * size), float(10 * size) },
                (Vector2){ float((10 * size) / 2), float((10 * size) / 2) }, rotation, WHITE);
    // DrawRectanglePro((Rectangle){ position.x, position.y, getAsteroidSize(), getAsteroidSize() }, origin, rotation, WHITE);
}


void Asteroid::destruct()
{
    rect = { position.x, position.y, float(size * 10), float(size * 10) };
    Animator::GetInstance().AddAnimation(asteroidID, std::make_shared<Animation>(
        ResourceManager::GetInstance().GetTextureRef("asteroid_destruct"),
        ResourceManager::GetInstance().GetTexture("asteroid_destruct").width / 7,
        ResourceManager::GetInstance().GetTexture("asteroid_destruct").height,
        0.1f,
        false,
        rect,
        false,
        WHITE
    ));
    Animator::GetInstance().SetOrigin(asteroidID, origin);
    Animator::GetInstance().SetPosition(asteroidID, position);
    Animator::GetInstance().SetRotation(asteroidID, rotation);
    Animator::GetInstance().Play(asteroidID);
}


