#pragma once
#include <raylib.h>
#include <memory>
#include <unordered_map>
#include <string>
#include <functional>
#include "Animation.hpp"

class Animator {
public:

    Animator();
    ~Animator();

    // Add animation with optional completion callback
    void AddAnimation(const std::string& name, 
                     std::shared_ptr<Animation> animation);

    // Control all animations
    void Play(const std::string& name);
    void StopAll();
    void PauseAll();
    void Update();
    void Draw(Rectangle dest, bool flipX, Color tint);

    // Individual animation control
    void Stop(const std::string& name);
    void Pause(const std::string& name);

private:
    struct AnimationData {
        std::shared_ptr<Animation> animation;
        bool isActive;
    };

    std::unordered_map<std::string, AnimationData> animations;
};