#pragma once
#include "Asteroid.hpp"
#include "Timer.hpp"
#include <vector>
#include <memory>

class AsteroidManager
{
private:

    std::vector<std::unique_ptr<Asteroid>> asteroids;
    Cooldown asteroidSpawnCooldown;
    
public:
    
    static AsteroidManager& GetInstance()
    {
        static AsteroidManager instance;
        return instance;
    }
    
    void update(Vector2 playerPos);
    void draw();
    void reset() { asteroids.clear(); };
    void removeAsteroid(int index) { asteroids.erase(asteroids.begin() + index); };
    std::vector<std::unique_ptr<Asteroid>>& getAsteroids() { return asteroids; };
    
private:
    
    void spawnAsteroid(Vector2 playerPos);
    AsteroidManager() = default;
    int asteroidCounter = 0; 
};