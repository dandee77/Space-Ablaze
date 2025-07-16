#pragma once
#include "raylib.h"
#include <string>
#include <array>

class Card {
private:

    Rectangle rect;
    std::string title;
    std::string description;
    
    float borderAnimationTime;
    float borderSpeed;
    float borderWidth;
    
    // Animation properties
    float introAnimationTime;
    float exitAnimationTime;
    bool isIntroPlaying;
    bool isExitPlaying;
    bool isAnimationComplete;
    bool hasExitStarted; 
    Rectangle originalRect;
    Rectangle targetRect;
    
    // Hover properties
    bool isHovered;
    float hoverScale;
    Rectangle baseRect;  

    std::array<Color, 4> borderColors;

    Color getBorderColorAtPosition(float normalizedPosition) const;
    void drawAnimatedBorder() const;
    void drawCardContent() const;
    
public:
    Card();
    Card(Vector2 position, Vector2 size, const std::string& cardTitle, const std::string& cardDescription);
    
    void update();
    void draw() const;

    Rectangle getRect() const { return rect; }
    Vector2 getPosition() const { return {rect.x, rect.y}; }
    Vector2 getSize() const { return {rect.width, rect.height}; }
    std::string getTitle() const { return title; }

    void setPosition(Vector2 position);
    void setSize(Vector2 size);
    void setTitle(const std::string& newTitle);
    void setDescription(const std::string& newDescription);

    bool isMouseHovered() const;
    bool isClicked() const;
    
    // Animation controls
    void startIntroAnimation();
    void startExitAnimation();
    bool isIntroComplete() const { return isAnimationComplete && !isIntroPlaying; }
    bool isExitComplete() const { return hasExitStarted && isAnimationComplete && !isExitPlaying; }
};