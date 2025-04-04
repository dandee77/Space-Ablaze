#pragma once
#include "raylib.h"


class Entity
{
protected:
    int health;
    int damage;
    float speed;
    Texture2D texture;
    Rectangle rect;
    float rotation;

public:
    Entity() = default;

    virtual bool update() = 0; // returns true if should shoot bullet
    virtual void draw() = 0;
    virtual ~Entity() = default;
    inline Vector2 getPosition() const { return {rect.x, rect.y}; }
    inline Vector2 getSize() const { return {rect.width, rect.height}; }
};