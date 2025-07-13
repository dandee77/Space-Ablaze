#include "AsteroidManager.hpp"
#include <vector>


#define ASTEROID_MAX_DISTANCE 150.0f

AsteroidManager::AsteroidManager()
{
    asteroidSpawnCooldown = Cooldown(1.5f); 
}

void AsteroidManager::update(Vector2 playerPos, const GameTimer& gameTimer)
{
    float elapsed = gameTimer.getElapsedTime();

    //? spawn cooldown drops over time, min 0.25s at 10 mins
    //? starts at 1.5s, drops to 0.25s over 10 minutes
    float newCooldown = std::max(0.25f, 1.5f - (elapsed / 600.0f) * 1.25f);
    asteroidSpawnCooldown.updateCooldownDuration(newCooldown);

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

    TraceLog(LOG_INFO, "Asteroid spawn rate: %f", newCooldown);
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