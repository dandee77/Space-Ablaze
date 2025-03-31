#pragma once

#include <raylib.h>
#include <vector>
#include <string>

class Animation 
{
public:
    Animation(Texture2D spriteSheet, int frameWidth, int frameHeight, float frameDuration, bool looping = true);
    ~Animation();
    
    void Update();
    void Draw(Rectangle dest, bool flipX, Color tint);
    void Reset();
    
    // Getters
    bool IsComplete() const;
    int GetCurrentFrame() const;
    Rectangle GetCurrentFrameRect() const;
    
    // Control
    void Play();
    void Pause();
    void Stop();

private:
    Texture2D spriteSheet;
    int frameWidth;
    int frameHeight;
    float frameDuration;
    float currentTime;
    int currentFrame;
    bool isPlaying;
    bool looping;
    bool complete;
    
    std::vector<Rectangle> frames;
};