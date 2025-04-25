#pragma once
#include "raylib.h"
#include "ResourceManager.hpp"
#include "raymath.h"
#include "Entity.hpp"
#include <string>


#define ASTEROID_ROT_SPEED_MIN 5
#define ASTEROID_ROT_SPEED_MAX 240
#define ASTEROID_RANDOM_ANGLE 30 * DEG2RAD
#define ASTEROID_SPEED_MIN 5
#define ASTEROID_SPEED_MAX 15


class Asteroid : public Entity
{
private:

    float rotationSpeed;
    float size;
    Vector2 playerPos;
    std::string asteroidID;
    static int asteroidCounter;
    
public:
    Asteroid(Vector2 playerPos);
    void update() override;
    void draw() override;
    void getPlayerPosition(Vector2 playerPos) { this->playerPos = playerPos; };
    // std::string getAsteroidID() const { return asteroidID; }
    float getAsteroidSize() const { return size * 2.5f; }
    void destruct();
};