#include "Animator.hpp"

// Singleton instance access
Animator& Animator::GetInstance() {
    static Animator instance;
    return instance;
}

void Animator::AddAnimation(const std::string& name, 
                          std::shared_ptr<Animation> animation) {
    animations[name] = {animation, false};
}

void Animator::Play(const std::string& name) {
    auto it = animations.find(name);
    if (it != animations.end()) {
        it->second.animation->Reset();
        it->second.animation->Play();
        it->second.isActive = true;
    }
}

void Animator::Update() {
    std::vector<std::string> toRemove;

    for (auto& [name, data] : animations) {
        if (data.isActive) {
            data.animation->Update();

            if (data.animation->IsComplete()) {
                toRemove.push_back(name);
            }
        }
    }

    for (const auto& name : toRemove) {
        animations.erase(name);
    }
}

void Animator::Draw() {
    for (auto& [name, data] : animations) {
        if (data.isActive) {
            data.animation->Draw();
        }
    }
}

void Animator::StopAll() {
    for (auto& [name, data] : animations) {
        data.animation->Stop();
        data.isActive = false;
    }
}

void Animator::PauseAll() {
    for (auto& [name, data] : animations) {
        data.animation->Pause();
    }
}

void Animator::Stop(const std::string& name) {
    auto it = animations.find(name);
    if (it != animations.end()) {
        it->second.animation->Stop();
        it->second.isActive = false;
    }
}

void Animator::Pause(const std::string& name) {
    auto it = animations.find(name);
    if (it != animations.end()) {
        it->second.animation->Pause();
    }
}

bool Animator::IsCompleted(const std::string& name) const {
    auto it = animations.find(name);
    return it->second.animation->IsComplete();
}