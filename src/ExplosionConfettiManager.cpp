#include "ExplosionConfettiManager.hpp"
#include <algorithm>
#include <cmath>

void ExplosionConfettiManager::startExplosion(Vector2 playerPosition)
{
    particles.clear();
    active = true;
    spawnTimer = 0.0f;
    spawnInterval = 0.03f;
    explosionDuration = 2.0f; 
    currentExplosionTime = 0.0f;
    explosionCenter = playerPosition;
    
    for (int i = 0; i < 50; i++) {
        particles.push_back(std::make_unique<ExplosionConfettiParticle>(explosionCenter));
    }
}

void ExplosionConfettiManager::stopExplosion()
{
    active = false;
}

void ExplosionConfettiManager::update()
{
    if (!active) return;
    
    float deltaTime = GetFrameTime();
    currentExplosionTime += deltaTime;
    
    spawnTimer += deltaTime;
    if (spawnTimer >= spawnInterval) {
        spawnTimer = 0.0f;
        
        int baseParticles = 12;
        int extraParticles = (int)(8.0f * sinf(currentExplosionTime * 3.0f)) + 8; 
        int particlesToSpawn = baseParticles + extraParticles;
        
        for (int i = 0; i < particlesToSpawn; i++) {
            particles.push_back(std::make_unique<ExplosionConfettiParticle>(explosionCenter));
        }
    }
    
    for (auto& particle : particles) {
        particle->update();
    }
    
    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
            [](const std::unique_ptr<ExplosionConfettiParticle>& particle) {
                return particle->isFinished();
            }),
        particles.end()
    );
}

void ExplosionConfettiManager::draw()
{
    for (auto& particle : particles) {
        particle->draw();
    }
}

void ExplosionConfettiManager::reset()
{
    particles.clear();
    active = false;
    spawnTimer = 0.0f;
    currentExplosionTime = 0.0f;
}
