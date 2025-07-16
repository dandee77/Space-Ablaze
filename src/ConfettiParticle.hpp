#pragma once
#include "raylib.h"

class ConfettiParticle
{
public:
    ConfettiParticle(Vector2 startPos);
    void update();
    void draw();
    bool isFinished() const { return finished; }

private:
    Vector2 position;
    Vector2 velocity;
    float rotation;
    float rotationSpeed;
    float originalSize;
    float currentSize;
    Color color;
    unsigned char originalAlpha;
    bool finished;
    bool isFading;
};
