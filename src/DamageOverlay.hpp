#pragma once
#include "raylib.h"
#include "Timer.hpp"

class DamageOverlay
{
public:
    static DamageOverlay& GetInstance() {
        static DamageOverlay instance;
        return instance;
    }
    
    void startDamageAnimation();
    void update();
    void draw();
    void reset();
    
private:
    DamageOverlay() = default;
    
    bool isActive;
    float animationTime;
    float animationDuration;
    float currentAlpha;
    
    void drawGradientRectangle(Rectangle rect, Color innerColor, Color outerColor, bool horizontal);
};
