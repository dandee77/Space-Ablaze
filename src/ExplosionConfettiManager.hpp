#pragma once
#include "ExplosionConfettiParticle.hpp"
#include <vector>
#include <memory>

class ExplosionConfettiManager
{
public:
    static ExplosionConfettiManager& GetInstance() {
        static ExplosionConfettiManager instance;
        return instance;
    }
    
    void startExplosion(Vector2 playerPosition);
    void stopExplosion();
    void update();
    void draw();
    void reset();
    bool isActive() const { return active; }
    
private:
    ExplosionConfettiManager() = default;
    std::vector<std::unique_ptr<ExplosionConfettiParticle>> particles;
    float spawnTimer;
    float spawnInterval;
    float explosionDuration;
    float currentExplosionTime;
    bool active;
    Vector2 explosionCenter;
};
