#include "DamageOverlay.hpp"
#include <cmath>
#include "raymath.h"

#define GAME_SCREEN_WIDTH 3840
#define GAME_SCREEN_HEIGHT 2160

void DamageOverlay::startDamageAnimation()
{
    isActive = true;
    animationTime = 0.0f;
    animationDuration = 3.0f; 
    currentAlpha = 0.0f;
}

void DamageOverlay::update()
{
    if (!isActive) return;
    
    animationTime += GetFrameTime();
    
    if (animationTime >= animationDuration) {
        isActive = false;
        currentAlpha = 0.0f;
        return;
    }
    
    float halfDuration = animationDuration * 0.5f;
    
    if (animationTime <= halfDuration) {
        float t = animationTime / halfDuration;
        currentAlpha = t * 150.0f; 
    } else {
        float t = (animationTime - halfDuration) / halfDuration;
        currentAlpha = (1.0f - t) * 150.0f;
    }

    if (currentAlpha < 0.0f) currentAlpha = 0.0f;
    if (currentAlpha > 255.0f) currentAlpha = 255.0f;
}

void DamageOverlay::draw()
{
    if (!isActive || currentAlpha <= 0.0f) return;
    
    unsigned char alpha = (unsigned char)currentAlpha;
    Color redTransparent = {255, 0, 0, alpha};
    Color transparent = {255, 0, 0, 0};
    
    float borderSize = 500.0f; 
    
    // Top 
    Rectangle topRect = {0, 0, GAME_SCREEN_WIDTH, borderSize};
    drawGradientRectangle(topRect, redTransparent, transparent, false);
    
    // Bottom 
    Rectangle bottomRect = {0, GAME_SCREEN_HEIGHT - borderSize, GAME_SCREEN_WIDTH, borderSize};
    drawGradientRectangle(bottomRect, transparent, redTransparent, false);
    
    // Left
    Rectangle leftRect = {0, 0, borderSize, GAME_SCREEN_HEIGHT};
    drawGradientRectangle(leftRect, redTransparent, transparent, true);
    
    // Right
    Rectangle rightRect = {GAME_SCREEN_WIDTH - borderSize, 0, borderSize, GAME_SCREEN_HEIGHT};
    drawGradientRectangle(rightRect, transparent, redTransparent, true);
}

void DamageOverlay::drawGradientRectangle(Rectangle rect, Color innerColor, Color outerColor, bool horizontal)
{
    int steps = 20; // no. of gradient steps for smooth transition
    
    if (horizontal) {
        // horizontal gradient
        float stepWidth = rect.width / steps;
        for (int i = 0; i < steps; i++) {
            float t = (float)i / (steps - 1);
            Color currentColor = {
                (unsigned char)Lerp(innerColor.r, outerColor.r, t),
                (unsigned char)Lerp(innerColor.g, outerColor.g, t),
                (unsigned char)Lerp(innerColor.b, outerColor.b, t),
                (unsigned char)Lerp(innerColor.a, outerColor.a, t)
            };
            
            Rectangle stepRect = {
                rect.x + i * stepWidth,
                rect.y,
                stepWidth + 1, // +1 to avoid gaps
                rect.height
            };
            DrawRectangleRec(stepRect, currentColor);
        }
    } else {
        // vertical gradient 
        float stepHeight = rect.height / steps;
        for (int i = 0; i < steps; i++) {
            float t = (float)i / (steps - 1);
            Color currentColor = {
                (unsigned char)Lerp(innerColor.r, outerColor.r, t),
                (unsigned char)Lerp(innerColor.g, outerColor.g, t),
                (unsigned char)Lerp(innerColor.b, outerColor.b, t),
                (unsigned char)Lerp(innerColor.a, outerColor.a, t)
            };
            
            Rectangle stepRect = {
                rect.x,
                rect.y + i * stepHeight,
                rect.width,
                stepHeight + 1 
            };
            DrawRectangleRec(stepRect, currentColor);
        }
    }
}

void DamageOverlay::reset()
{
    isActive = false;
    animationTime = 0.0f;
    currentAlpha = 0.0f;
}
