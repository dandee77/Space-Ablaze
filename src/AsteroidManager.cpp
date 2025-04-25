#include "AsteroidManager.hpp"

#define ASTEROID_MAX_DISTANCE 150.0f

AsteroidManager::AsteroidManager()
{
    asteroidSpawnCooldown = Cooldown(0.1f); 
}

void AsteroidManager::update(Vector2 playerPos)
{
    if (!asteroidSpawnCooldown.isOnCooldown())
    {
        int spawns = 1 + GetRandomValue(0, 2); 
        for (int i = 0; i < spawns; i++)
            spawnAsteroid(playerPos);
        asteroidSpawnCooldown.startCooldown();
    }

    for (int i = 0; i < asteroids.size(); i++)
    {
        asteroids[i]->getPlayerPosition(playerPos); 
        asteroids[i]->update(); 
        float dist = Vector2Distance(asteroids[i]->getPosition(), playerPos);
        if (dist > ASTEROID_MAX_DISTANCE) 
        {
            asteroids.erase(asteroids.begin() + i);
            i--;
        }
    }

    TraceLog(LOG_INFO, "Asteroid count: %d", asteroids.size());
}

void AsteroidManager::draw()
{
    for (auto& asteroid : asteroids)
    {
        asteroid->draw();
    }
}

void AsteroidManager::spawnAsteroid(Vector2 playerPos)
{
    asteroids.push_back(std::make_unique<Asteroid>(playerPos));
}

void AsteroidManager::removeAsteroid(int index)
{
    asteroids[index]->destruct(); 
    asteroids.erase(asteroids.begin() + index);
}