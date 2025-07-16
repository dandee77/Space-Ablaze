#include "ConfettiManager.hpp"
#include <algorithm>

#define GAME_SCREEN_WIDTH 3840
#define GAME_SCREEN_HEIGHT 2160

void ConfettiManager::startConfetti()
{
    particles.clear();
    isActive = true;
    spawnTimer = 0.0f;
    spawnInterval = 0.01f; 
}

void ConfettiManager::stopConfetti()
{
    isActive = false;
}

void ConfettiManager::update()
{
    if (!isActive) return;
    
    float deltaTime = GetFrameTime();
    
    spawnTimer += deltaTime;
    if (spawnTimer >= spawnInterval) {
        spawnTimer = 0.0f;
        
        for (int i = 0; i < 3; i++) {
            Vector2 spawnPos = {
                (float)GetRandomValue(0, GAME_SCREEN_WIDTH),
                (float)GetRandomValue(-50, -10) 
            };
            particles.push_back(std::make_unique<ConfettiParticle>(spawnPos));
        }
    }
    
    for (auto& particle : particles) {
        particle->update();
    }
    
    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
            [](const std::unique_ptr<ConfettiParticle>& particle) {
                return particle->isFinished();
            }),
        particles.end()
    );
}

void ConfettiManager::draw()
{
    for (auto& particle : particles) {
        particle->draw();
    }
}

void ConfettiManager::reset()
{
    particles.clear();
    isActive = false;
    spawnTimer = 0.0f;
}
