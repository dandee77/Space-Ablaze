#pragma once
#include "raylib.h"

class ExplosionConfettiParticle
{
public:
    ExplosionConfettiParticle(Vector2 startPos);
    void update();
    void draw();
    bool isFinished() const { return finished; }

private:
    Vector2 position;
    Vector2 velocity;
    float rotation;
    float rotationSpeed;
    float currentSize;
    float maxSize;
    float growthRate;
    Color color;
    unsigned char originalAlpha;
    bool finished;
    bool isGrowing;
    float lifetime;
    float maxLifetime;
};
