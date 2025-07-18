#include "GameOverOverlay.hpp"

#define GAME_SCREEN_WIDTH 3840
#define GAME_SCREEN_HEIGHT 2160

void GameOverOverlay::startOverlay()
{
    active = true;
    timer = 0.0f;
    delayDuration = 5.0f;
    triggered = false;
    screenWidth = GAME_SCREEN_WIDTH;
    screenHeight = GAME_SCREEN_HEIGHT;
    overlayPosition = -screenHeight; 
    slideSpeed = 8000.0f; 
    targetAlpha = 0.7f; 
    currentAlpha = 0.0f;
    
    textAnimationStarted = false;
    textSlidePosition = -200.0f; 
    textSlideSpeed = 2000.0f;
    textTargetPosition = 200.0f;
}

void GameOverOverlay::update()
{
    if (!active) return;
    
    float deltaTime = GetFrameTime();
    timer += deltaTime;
    
    if (!triggered) {
        if (timer >= delayDuration || GetKeyPressed() != 0) {
            triggered = true;
        }
    }
    
    if (triggered && overlayPosition < 0) {
        overlayPosition += slideSpeed * deltaTime;
        if (overlayPosition >= 0) {
            overlayPosition = 0; 
        }

        float slideProgress = (overlayPosition + screenHeight) / screenHeight;
        currentAlpha = targetAlpha * slideProgress;
    }
    
    if (overlayPosition >= 0 && !textAnimationStarted) {
        textAnimationStarted = true;
    }
    
    if (textAnimationStarted && textSlidePosition < textTargetPosition) {
        textSlidePosition += textSlideSpeed * deltaTime;
        if (textSlidePosition >= textTargetPosition) {
            textSlidePosition = textTargetPosition;
        }
    }
}

void GameOverOverlay::draw()
{
    if (!active || !triggered) return;
    
    Rectangle overlayRect = {
        0, 
        overlayPosition, 
        screenWidth, 
        screenHeight
    };
    
    Color overlayColor = {0, 0, 0, (unsigned char)(currentAlpha * 255)};
    DrawRectangleRec(overlayRect, overlayColor);
}

void GameOverOverlay::reset()
{
    active = false;
    timer = 0.0f;
    triggered = false;
    overlayPosition = -GAME_SCREEN_HEIGHT;
    currentAlpha = 0.0f;
    textAnimationStarted = false;
    textSlidePosition = -200.0f;
}
