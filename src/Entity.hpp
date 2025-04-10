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
    Vector2 velocity;

public:
    Entity() = default;

    virtual bool update() {  return false; }; 
    virtual void draw(Texture2D entityTexture) {};
    virtual ~Entity() = default;
    virtual Vector2 getPosition() const { return {rect.x, rect.y}; }
    inline Vector2 getSize() const { return {rect.width, rect.height}; }
    inline float getRotation() const { return rotation; }
    inline Vector2 getVelocity() const { return velocity; }
};