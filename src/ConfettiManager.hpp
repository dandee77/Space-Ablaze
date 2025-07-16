#pragma once
#include "ConfettiParticle.hpp"
#include <vector>
#include <memory>

class ConfettiManager
{
public:
    static ConfettiManager& GetInstance() {
        static ConfettiManager instance;
        return instance;
    }
    
    void startConfetti();
    void stopConfetti();
    void update();
    void draw();
    void reset();
    
private:
    ConfettiManager() = default;
    std::vector<std::unique_ptr<ConfettiParticle>> particles;
    float spawnTimer;
    float spawnInterval;
    bool isActive;
};
