#pragma once
#include "raylib.h"

class GameOverOverlay
{
public:
    static GameOverOverlay& GetInstance() {
        static GameOverOverlay instance;
        return instance;
    }
    
    void startOverlay();
    void update();
    void draw();
    void reset();
    bool isActive() const { return active; }
    bool isFullyVisible() const { return overlayPosition >= 0; }
    bool shouldShowText() const { return overlayPosition >= 0; }
    float getTextYPosition() const { return textSlidePosition; }
    
private:
    GameOverOverlay() = default;
    
    bool active;
    float timer;
    float delayDuration; 
    bool triggered;
    float overlayPosition; 
    float slideSpeed;
    float screenWidth;
    float screenHeight;
    float targetAlpha;
    float currentAlpha;
    
    bool textAnimationStarted;
    float textSlidePosition;
    float textSlideSpeed;
    float textTargetPosition;
};
