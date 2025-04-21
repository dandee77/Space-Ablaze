#pragma once

#include <raylib.h>
#include <vector>
#include <string>


//TODO: MAKE TEXTURE POINTER INSTEAD OF TEXTURE2D

class Animation 
{
public:
    Animation(const Texture2D& spriteSheet, int frameWidth, int frameHeight, float frameDuration, bool looping, Rectangle dest, bool flipX, Color tint);
    ~Animation();
    
    void Update();
    void Draw();
    void Reset();
    
    // Getters
    bool IsComplete() const;
    int GetCurrentFrame() const;
    Rectangle GetCurrentFrameRect() const;
    
    // Control
    void Play();
    void Pause();
    void Stop();

    void SetRotation(float rotation);
    void SetOrigin(Vector2 origin);
    void SetTint(Color tint);
    void SetPosition(Vector2 position);

private:
    const Texture2D& spriteSheet;
    int frameWidth;
    int frameHeight;
    float frameDuration;
    float currentTime;
    int currentFrame;
    bool isPlaying;
    bool looping;
    bool complete;
    Rectangle dest;
    bool flipX;
    Color tint;
    float rotation;
    Vector2 origin;
    
    std::vector<Rectangle> frames;
};