#include "ExplosionConfettiParticle.hpp"
#include <cmath>

ExplosionConfettiParticle::ExplosionConfettiParticle(Vector2 startPos)
    : position(startPos), finished(false), isGrowing(true), lifetime(0.0f)
{
    float angle = (float)GetRandomValue(0, 360) * DEG2RAD;
    float speed = (float)GetRandomValue(100, 300); 
    
    velocity = {
        cosf(angle) * speed,
        sinf(angle) * speed
    };

    rotation = (float)GetRandomValue(0, 360);
    rotationSpeed = (float)GetRandomValue(-360, 360); 
    
    currentSize = (float)GetRandomValue(1, 2);
    maxSize = (float)GetRandomValue(2, 4); 
    growthRate = (float)GetRandomValue(5, 10); 
    
    maxLifetime = (float)GetRandomValue(5, 10) / 10.0f; 

    Color colors[] = {
        {255, 50, 50, 255},   // Bright red
        {50, 255, 50, 255},   // Bright green  
        {50, 50, 255, 255},   // Bright blue
        {255, 255, 50, 255},  // Bright yellow
        {255, 50, 255, 255},  // Bright magenta
        {50, 255, 255, 255},  // Bright cyan
        {255, 150, 50, 255},  // Bright orange
        {150, 50, 255, 255},  // Bright purple
        {255, 255, 255, 255}, // White
        {255, 100, 150, 255}, // Pink
    };
    
    color = colors[GetRandomValue(0, 9)];
    originalAlpha = color.a;
}

void ExplosionConfettiParticle::update()
{
    if (finished) return;
    
    float deltaTime = GetFrameTime();
    lifetime += deltaTime;
    
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
    
    rotation += rotationSpeed * deltaTime;
    
    velocity.x *= 0.98f;
    velocity.y *= 0.98f;
    
    if (isGrowing && currentSize < maxSize) {
        currentSize += growthRate * deltaTime;
        if (currentSize >= maxSize) {
            isGrowing = false;
        }
    }

    float fadeStart = maxLifetime * 0.6f; 
    if (lifetime > fadeStart) {
        float fadeProgress = (lifetime - fadeStart) / (maxLifetime - fadeStart);
        color.a = (unsigned char)((float)originalAlpha * (1.0f - fadeProgress));
    }
    
    if (lifetime >= maxLifetime || color.a <= 0) {
        finished = true;
    }
}

void ExplosionConfettiParticle::draw()
{
    if (finished || currentSize <= 0.5f || color.a <= 0) return;
    
    Vector2 origin = {currentSize / 2.0f, currentSize / 2.0f};
    Rectangle rect = {position.x, position.y, currentSize, currentSize};
    
    DrawRectanglePro(rect, origin, rotation, color);
}
