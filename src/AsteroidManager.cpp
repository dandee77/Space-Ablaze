#include "AsteroidManager.hpp"

#define ASTEROID_MAX_DISTANCE 150.0f

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
        if (!asteroids[i]->active) 
        {
            asteroids.erase(asteroids.begin() + i);
            i--;
        }
    }
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