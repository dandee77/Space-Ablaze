#include "Animation.hpp"

Animation::Animation(const Texture2D& spriteSheet, int frameWidth, int frameHeight, 
                   float frameDuration, bool looping, Rectangle dest, bool flipX, Color tint) 
    : spriteSheet(spriteSheet), frameWidth(frameWidth), frameHeight(frameHeight),
      frameDuration(frameDuration), looping(looping), dest(dest), flipX(flipX), 
      tint(tint), rotation(0), origin({0, 0}), complete(false) {
    
    // Calculate how many frames we have
    int framesX = spriteSheet.width / frameWidth;
    int framesY = spriteSheet.height / frameHeight;
    
    // Create frame rectangles
    for (int y = 0; y < framesY; y++) {
        for (int x = 0; x < framesX; x++) {
            frames.push_back({
                (float)(x * frameWidth),
                (float)(y * frameHeight),
                (float)frameWidth,
                (float)frameHeight
            });
        }
    }
    
    Reset();
}

Animation::~Animation() {
    // Note: We don't unload the texture here since it might be shared
}

#include <iostream>

void Animation::Update() {
    if (!isPlaying) return;
    
    currentTime += GetFrameTime();
    if (currentTime >= frameDuration) {
        currentTime = 0;
        currentFrame++;
        
        if (currentFrame >= (int)frames.size()) {
            if (looping) {
                currentFrame = 0;
            } else {
                currentFrame = (int)frames.size() - 1;
                complete = true;
            }
        }
    }
}

void Animation::Draw() {
    Rectangle source = frames[currentFrame];
        
    if (flipX) {
        source.width *= -1; 
    }
    
    DrawTexturePro(spriteSheet, source, dest, origin, rotation, tint);
}

void Animation::SetRotation(float rotation) {
    this->rotation = rotation;
}

void Animation::SetOrigin(Vector2 origin) {
    this->origin = origin;
}

void Animation::SetTint(Color tint) {
    this->tint = tint;
}

void Animation::SetPosition(Vector2 position) {
    dest.x = position.x;
    dest.y = position.y;
}

void Animation::Reset() {
    currentFrame = 0;
    currentTime = 0;
    isPlaying = true;
    complete = false;
}

bool Animation::IsComplete() const { return complete; }
int Animation::GetCurrentFrame() const { return currentFrame; }
Rectangle Animation::GetCurrentFrameRect() const { return frames[currentFrame]; }

void Animation::Play() { isPlaying = true; }
void Animation::Pause() { isPlaying = false; }
void Animation::Stop() { 
    isPlaying = false; 
    Reset();
}