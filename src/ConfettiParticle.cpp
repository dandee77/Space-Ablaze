#include "ConfettiParticle.hpp"
#include <cmath>

#define SCREEN_MIDDLE 1400.0f

ConfettiParticle::ConfettiParticle(Vector2 startPos)
    : position(startPos), finished(false), isFading(false)
{
    velocity = {
        (float)GetRandomValue(-30, 30), 
        (float)GetRandomValue(50, 100) 
    };

    rotation = (float)GetRandomValue(0, 360);
    rotationSpeed = (float)GetRandomValue(-180, 180); 
    
    originalSize = (float)GetRandomValue(30, 75);
    currentSize = originalSize;

    Color colors[] = {
        {255, 100, 100, 255}, // Light red
        {100, 255, 100, 255}, // Light green
        {100, 100, 255, 255}, // Light blue
        {255, 255, 100, 255}, // Yellow
        {255, 100, 255, 255}, // Magenta
        {100, 255, 255, 255}, // Cyan
        {255, 150, 100, 255}, // Orange
        {200, 100, 255, 255}, // Purple
    };
    
    color = colors[GetRandomValue(0, 7)];
    originalAlpha = color.a;
}

void ConfettiParticle::update()
{
    if (finished) return;
    
    float deltaTime = GetFrameTime();
    
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
    
    rotation += rotationSpeed * deltaTime;
    
    if (position.y >= SCREEN_MIDDLE) {
        isFading = true;
    }
    
    float shrinkRate = 10.0f * deltaTime; // shrinks 20 px per second
    currentSize -= shrinkRate;
    if (currentSize < 0.0f) currentSize = 0.0f;
    
    velocity.y += 80.0f * deltaTime; // gravity acceleration
    
    // air resistance
    velocity.x *= 0.995f;
    velocity.y *= 0.998f;
    
    if (isFading) {
        float fadeRate = 255.0f * deltaTime; // fade 255s alpha per second
        color.a = (unsigned char)((float)color.a - fadeRate);
        if (color.a <= 0) {
            color.a = 0;
            finished = true;
        }
    }
    
    if (currentSize <= 0.5f || color.a <= 0) {
        finished = true;
    }
}

void ConfettiParticle::draw()
{
    if (finished || currentSize <= 0.5f || color.a <= 0) return;
    
    Vector2 origin = {currentSize / 2.0f, currentSize / 2.0f};
    Rectangle rect = {position.x, position.y, currentSize, currentSize};
    
    DrawRectanglePro(rect, origin, rotation, color);
}
