#pragma once
#include <raylib.h>
#include <memory>
#include <unordered_map>
#include <string>
#include <functional>
#include "Animation.hpp"

class Animator {
public:
   
    Animator(const Animator&) = delete;
    Animator& operator=(const Animator&) = delete;

  
    static Animator& GetInstance();

   
    void AddAnimation(const std::string& name, 
                     std::shared_ptr<Animation> animation);

 
    void Play(const std::string& name);
    void StopAll();
    void PauseAll();
    void Update();
    void Draw();
    void Remove(const std::string& name);


  
    void Stop(const std::string& name);
    void Pause(const std::string& name);
    bool IsCompleted(const std::string& name) const;
    void SetRotation(const std::string& name, float rotation);
    void SetOrigin(const std::string& name, Vector2 origin);
    void SetTint(const std::string& name, Color tint);
    void SetPosition(const std::string& name, Vector2 position);

private:
  
    Animator() = default;
    ~Animator() = default;

    struct AnimationData {
        std::shared_ptr<Animation> animation;
        bool isActive;
    };

    std::unordered_map<std::string, AnimationData> animations;
};