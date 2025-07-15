#include "Card.hpp"
#include "ResourceManager.hpp"
#include "raymath.h"
#include <cmath>

// Easing function for bouncy animation
float easeOutBounce(float t) {
    if (t < 1.0f / 2.75f) {
        return 7.5625f * t * t;
    } else if (t < 2.0f / 2.75f) {
        t -= 1.5f / 2.75f;
        return 7.5625f * t * t + 0.75f;
    } else if (t < 2.5f / 2.75f) {
        t -= 2.25f / 2.75f;
        return 7.5625f * t * t + 0.9375f;
    } else {
        t -= 2.625f / 2.75f;
        return 7.5625f * t * t + 0.984375f;
    }
}

// Easing function for smooth in-out animation
float easeInOutQuad(float t) {
    return t < 0.5f ? 2.0f * t * t : -1.0f + (4.0f - 2.0f * t) * t;
}

Card::Card() {
    rect = {0, 0, 200, 300};
    title = "Card Title";
    description = "Card Description";
    borderAnimationTime = 0.0f;
    borderSpeed = 5.0f;
    borderWidth = 20.0f;
    
    introAnimationTime = 0.0f;
    exitAnimationTime = 0.0f;
    isIntroPlaying = false;
    isExitPlaying = false;
    isAnimationComplete = false;
    hasExitStarted = false;
    originalRect = rect;
    targetRect = rect;
    
    isHovered = false;
    hoverScale = 1.0f;
    baseRect = rect;
    
    // Generate 4 random colors for the border
    borderColors[0] = {(unsigned char)(rand() % 256), (unsigned char)(rand() % 256), (unsigned char)(rand() % 256), 255};
    borderColors[1] = {(unsigned char)(rand() % 256), (unsigned char)(rand() % 256), (unsigned char)(rand() % 256), 255};
    borderColors[2] = {(unsigned char)(rand() % 256), (unsigned char)(rand() % 256), (unsigned char)(rand() % 256), 255};
    borderColors[3] = {(unsigned char)(rand() % 256), (unsigned char)(rand() % 256), (unsigned char)(rand() % 256), 255};
}

Card::Card(Vector2 position, Vector2 size, const std::string& cardTitle, const std::string& cardDescription) {
    rect = {position.x, position.y, size.x, size.y};
    title = cardTitle;
    description = cardDescription;
    borderAnimationTime = 0.0f;
    borderSpeed = 5.0f;
    borderWidth = 20.0f;
    
    introAnimationTime = 0.0f;
    exitAnimationTime = 0.0f;
    isIntroPlaying = false;
    isExitPlaying = false;
    isAnimationComplete = false;
    hasExitStarted = false;
    originalRect = rect;
    targetRect = rect;
    
    isHovered = false;
    hoverScale = 1.0f;
    baseRect = rect;
    
    borderColors[0] = {(unsigned char)(rand() % 256), (unsigned char)(rand() % 256), (unsigned char)(rand() % 256), 255};
    borderColors[1] = {(unsigned char)(rand() % 256), (unsigned char)(rand() % 256), (unsigned char)(rand() % 256), 255};
    borderColors[2] = {(unsigned char)(rand() % 256), (unsigned char)(rand() % 256), (unsigned char)(rand() % 256), 255};
    borderColors[3] = {(unsigned char)(rand() % 256), (unsigned char)(rand() % 256), (unsigned char)(rand() % 256), 255};
}

void Card::update() {
    borderAnimationTime += GetFrameTime() * borderSpeed;
    if (borderAnimationTime >= 8.0f) {
        borderAnimationTime = 0.0f;
    }
    
    // Handle hover scaling
    bool currentlyHovered = isMouseHovered();
    if (currentlyHovered && !isHovered) {
        isHovered = true;
        baseRect = rect;  // Store current rect before scaling
    } else if (!currentlyHovered && isHovered) {
        isHovered = false;
    }
    
    // Smooth hover scaling
    float targetScale = isHovered ? 1.1f : 1.0f;
    hoverScale = Lerp(hoverScale, targetScale, 8.0f * GetFrameTime());
    
    // Apply hover scaling (only when not animating)
    if (!isIntroPlaying && !isExitPlaying && isAnimationComplete) {
        float scaledWidth = baseRect.width * hoverScale;
        float scaledHeight = baseRect.height * hoverScale;
        
        rect.width = scaledWidth;
        rect.height = scaledHeight;
        rect.x = baseRect.x + (baseRect.width - scaledWidth) * 0.5f;
        rect.y = baseRect.y + (baseRect.height - scaledHeight) * 0.5f;
    }
    
    // Handle intro animation - horizontal grow from center
    if (isIntroPlaying) {
        introAnimationTime += GetFrameTime();
        float duration = 0.8f; // Animation duration in seconds
        
        if (introAnimationTime >= duration) {
            introAnimationTime = duration;
            isIntroPlaying = false;
            isAnimationComplete = true;
            rect = targetRect;
            baseRect = rect; 
        } else {
            float t = introAnimationTime / duration;
            float easedT = easeOutBounce(t);
            
            // Animate width growing from center
            float currentWidth = easedT * targetRect.width;
            rect.width = currentWidth;
            rect.height = targetRect.height;
            rect.x = targetRect.x + (targetRect.width - currentWidth) * 0.5f;
            rect.y = targetRect.y;
        }
    }
    
    // Handle exit animation - horizontal shrink to center
    if (isExitPlaying) {
        exitAnimationTime += GetFrameTime();
        float duration = 0.3f; // Animation duration in seconds
        
        if (exitAnimationTime >= duration) {
            exitAnimationTime = duration;
            isExitPlaying = false;
            isAnimationComplete = true;
            rect.width = 0;
            rect.height = targetRect.height;
        } else {
            float t = exitAnimationTime / duration;
            float easedT = easeInOutQuad(t);
            
            // First grow a bit then shrink horizontally
            float scale;
            if (t < 0.2f) {
                // Grow phase (0-20% of animation)
                scale = 1.0f + (t / 0.2f) * 0.1f; // Grow to 110%
            } else {
                // Shrink phase (20-100% of animation)
                float shrinkT = (t - 0.2f) / 0.8f;
                scale = 1.1f - shrinkT * 1.1f; // Shrink from 110% to 0%
            }
            
            float currentWidth = targetRect.width * scale;
            rect.width = currentWidth;
            rect.height = targetRect.height;
            rect.x = targetRect.x + (targetRect.width - currentWidth) * 0.5f;
            rect.y = targetRect.y;
        }
    }
}

void Card::draw() const {
    DrawRectangleRec(rect, Color{20, 20, 30, 255});
    drawAnimatedBorder();
    drawCardContent();
}

Color Card::getBorderColorAtPosition(float normalizedPosition) const {
    float colorPosition = fmodf(normalizedPosition + (borderAnimationTime / 8.0f), 1.0f);
    
    if (colorPosition < 0.25f) {
        float t = colorPosition * 4.0f;
        return {
            (unsigned char)Lerp(borderColors[0].r, borderColors[1].r, t),
            (unsigned char)Lerp(borderColors[0].g, borderColors[1].g, t),
            (unsigned char)Lerp(borderColors[0].b, borderColors[1].b, t),
            255
        };
    } else if (colorPosition < 0.5f) {
        float t = (colorPosition - 0.25f) * 4.0f;
        return {
            (unsigned char)Lerp(borderColors[1].r, borderColors[2].r, t),
            (unsigned char)Lerp(borderColors[1].g, borderColors[2].g, t),
            (unsigned char)Lerp(borderColors[1].b, borderColors[2].b, t),
            255
        };
    } else if (colorPosition < 0.75f) {
        float t = (colorPosition - 0.5f) * 4.0f;
        return {
            (unsigned char)Lerp(borderColors[2].r, borderColors[3].r, t),
            (unsigned char)Lerp(borderColors[2].g, borderColors[3].g, t),
            (unsigned char)Lerp(borderColors[2].b, borderColors[3].b, t),
            255
        };
    } else {
        float t = (colorPosition - 0.75f) * 4.0f;
        return {
            (unsigned char)Lerp(borderColors[3].r, borderColors[0].r, t),
            (unsigned char)Lerp(borderColors[3].g, borderColors[0].g, t),
            (unsigned char)Lerp(borderColors[3].b, borderColors[0].b, t),
            255
        };
    }
}

void Card::drawAnimatedBorder() const {
    float perimeter = 2.0f * (rect.width + rect.height);
    int segments = 100;
    
    for (int i = 0; i < segments; i++) {
        float t1 = (float)i / segments;
        float t2 = (float)(i + 1) / segments;
        
        Vector2 pos1, pos2;
        
        if (t1 < rect.width / perimeter) {
            pos1 = {rect.x + t1 * perimeter, rect.y};
        }
        else if (t1 < (rect.width + rect.height) / perimeter) {
            float edgeT = (t1 * perimeter - rect.width) / rect.height;
            pos1 = {rect.x + rect.width, rect.y + edgeT * rect.height};
        }
        else if (t1 < (2.0f * rect.width + rect.height) / perimeter) {
            float edgeT = (t1 * perimeter - rect.width - rect.height) / rect.width;
            pos1 = {rect.x + rect.width - edgeT * rect.width, rect.y + rect.height};
        }
        else {
            float edgeT = (t1 * perimeter - 2.0f * rect.width - rect.height) / rect.height;
            pos1 = {rect.x, rect.y + rect.height - edgeT * rect.height};
        }
        
        if (t2 < rect.width / perimeter) {
            pos2 = {rect.x + t2 * perimeter, rect.y};
        }
        else if (t2 < (rect.width + rect.height) / perimeter) {
            float edgeT = (t2 * perimeter - rect.width) / rect.height;
            pos2 = {rect.x + rect.width, rect.y + edgeT * rect.height};
        }
        else if (t2 < (2.0f * rect.width + rect.height) / perimeter) {
            float edgeT = (t2 * perimeter - rect.width - rect.height) / rect.width;
            pos2 = {rect.x + rect.width - edgeT * rect.width, rect.y + rect.height};
        }
        else {
            float edgeT = (t2 * perimeter - 2.0f * rect.width - rect.height) / rect.height;
            pos2 = {rect.x, rect.y + rect.height - edgeT * rect.height};
        }
        
        Color segmentColor = getBorderColorAtPosition(t1);
        DrawLineEx(pos1, pos2, borderWidth, segmentColor);
    }
}

void Card::drawCardContent() const {
    // Hide content during exit animation
    if (isExitPlaying) {
        return;
    }
    
    // Hide content during intro animation for first 0.2 seconds
    if (isIntroPlaying && introAnimationTime < 0.2f) {
        return;
    }
    
    // Scale font sizes based on hover
    float titleSize = 100 * hoverScale;
    float descSize = 64 * hoverScale;
    
    Vector2 titlePos = {rect.x + 50, rect.y + 30};
    DrawTextEx(ResourceManager::GetInstance().GetFont("primary_font"), title.c_str(), titlePos, titleSize, 1.0f, RAYWHITE);
    
    Vector2 descPos = {rect.x + 50, rect.y + 150 * hoverScale};
    DrawTextEx(ResourceManager::GetInstance().GetFont("secondary_font"), description.c_str(), descPos, descSize, 1.0f, RAYWHITE);
    
    Rectangle innerRect = {rect.x + borderWidth, rect.y + borderWidth, 
                          rect.width - 2 * borderWidth, rect.height - 2 * borderWidth};
    DrawRectangleLinesEx(innerRect, 1, {60, 60, 80, 255});
}

// Setters
void Card::setPosition(Vector2 position) {
    rect.x = position.x;
    rect.y = position.y;
}

void Card::setSize(Vector2 size) {
    rect.width = size.x;
    rect.height = size.y;
}

void Card::setTitle(const std::string& newTitle) {
    title = newTitle;
}

void Card::setDescription(const std::string& newDescription) {
    description = newDescription;
}

// Interaction
bool Card::isMouseHovered() const {
    Vector2 mousePos = GetMousePosition();
    return CheckCollisionPointRec(mousePos, rect);
}

bool Card::isClicked() const {
    return isMouseHovered() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

// Animation controls
void Card::startIntroAnimation() {
    isIntroPlaying = true;
    isExitPlaying = false;
    isAnimationComplete = false;
    hasExitStarted = false;  // Reset exit flag for new intro
    introAnimationTime = 0.0f;
    targetRect = rect;
    baseRect = rect;
    hoverScale = 1.0f;  // Reset hover scale
    
    // Start with zero width at center
    rect.width = 0;
    rect.x = targetRect.x + targetRect.width * 0.5f;
}

void Card::startExitAnimation() {
    isIntroPlaying = false;
    isExitPlaying = true;
    isAnimationComplete = false;
    hasExitStarted = true;  // Mark that exit animation was started
    exitAnimationTime = 0.0f;
    targetRect = rect;
    hoverScale = 1.0f;  // Reset hover scale
}