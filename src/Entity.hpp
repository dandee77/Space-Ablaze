#pragma once
#include "raylib.h"


class Entity
{
protected:
    int health;
    int damage;
    float speed;
    Rectangle rect;
    float rotation;
    Vector2 velocity;
    Vector2 position;
    Vector2 origin;
    

public:
    Entity() = default;

    virtual void update() = 0;
    virtual void draw() = 0;
    virtual ~Entity() = default;
    inline Vector2 getPosition() const { return position; }
    inline Vector2 getSize() const { return {rect.width, rect.height}; }
    inline float getRotation() const { return rotation; }
    inline Vector2 getVelocity() const { return velocity; }
    inline int getHealth() const { return health; }
};