#include "Animation.hpp"

Animation::Animation(Texture2D spriteSheet, int frameWidth, int frameHeight, 
                   float frameDuration, bool looping) 
    : spriteSheet(spriteSheet), frameWidth(frameWidth), frameHeight(frameHeight),
      frameDuration(frameDuration), looping(looping) {
    
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

void Animation::Update() {
    float deltaTime = GetFrameTime();
    if (!isPlaying || complete) return;
    
    currentTime += deltaTime;
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

void Animation::Draw(Rectangle dest, bool flipX, Color tint) {
    Rectangle source = frames[currentFrame];
    
    Vector2 origin = { 0, 0 };
    float rotation = 0;
    
    if (flipX) {
        source.width *= -1; // Flip the source rectangle
    }
    
    DrawTexturePro(spriteSheet, source, dest, origin, rotation, tint);
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