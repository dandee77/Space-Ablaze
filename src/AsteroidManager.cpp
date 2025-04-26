#include "AsteroidManager.hpp"
#include <vector>


#define ASTEROID_MAX_DISTANCE 150.0f

AsteroidManager::AsteroidManager()
{
    asteroidSpawnCooldown = Cooldown(0.1f); 
}

void AsteroidManager::update(Vector2 playerPos)
{
    if (!asteroidSpawnCooldown.isOnCooldown()) {
        int spawns = 1 + GetRandomValue(0, 2); 
        for (int i = 0; i < spawns; i++)
            spawnAsteroid(playerPos);
        asteroidSpawnCooldown.startCooldown();
    }

    std::vector<std::string> toRemove;
    for (auto& [id, asteroid] : asteroids) {
        asteroid->getPlayerPosition(playerPos);
        asteroid->update();
        float dist = Vector2Distance(asteroid->getPosition(), playerPos);
        if (dist > ASTEROID_MAX_DISTANCE) {
            toRemove.push_back(id);
        }
    }

    for (auto& id : toRemove) {
        asteroids.erase(id);
    }

    TraceLog(LOG_INFO, "Asteroid count: %d", asteroids.size());
}

void AsteroidManager::draw()
{
    for (auto& [id, asteroid] : asteroids) {
        asteroid->draw();
    }
}

void AsteroidManager::spawnAsteroid(Vector2 playerPos)
{
    std::unique_ptr<Asteroid> asteroid = std::make_unique<Asteroid>(playerPos);
    asteroids[asteroid->getAsteroidID()] = std::move(asteroid);
}

void AsteroidManager::removeAsteroid(const std::string& id)
{
    asteroids[id]->destruct();
    asteroids.erase(id);
}