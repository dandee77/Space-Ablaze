#pragma once
#include <raylib.h>
#include <memory>
#include <unordered_map>
#include <string>
#include <functional>
#include "Animation.hpp"

class Animator {
public:
    // Delete copy constructor and assignment operator
    Animator(const Animator&) = delete;
    Animator& operator=(const Animator&) = delete;

    // Singleton accessor
    static Animator& GetInstance();

    // Add animation with optional completion callback
    void AddAnimation(const std::string& name, 
                     std::shared_ptr<Animation> animation);

    // Control all animations
    void Play(const std::string& name);
    void StopAll();
    void PauseAll();
    void Update();
    void Draw();

    // Individual animation control
    void Stop(const std::string& name);
    void Pause(const std::string& name);

    bool IsCompleted(const std::string& name) const;

private:
    // Private constructor for singleton
    Animator() = default;
    ~Animator() = default;

    struct AnimationData {
        std::shared_ptr<Animation> animation;
        bool isActive;
    };

    std::unordered_map<std::string, AnimationData> animations;
};