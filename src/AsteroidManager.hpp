#pragma once
#include "Asteroid.hpp"
#include "Timer.hpp"
#include <unordered_map>
#include <memory>
#include "GameTimer.hpp"

class AsteroidManager
{
private:

    std::unordered_map<std::string, std::unique_ptr<Asteroid>> asteroids;
    Cooldown asteroidSpawnCooldown;
    
public:
    
    static AsteroidManager& GetInstance()
    {
        static AsteroidManager instance;
        return instance;
    }
    
    void update(Vector2 playerPos, const GameTimer& gameTimer);
    void draw();
    void reset() { asteroids.clear(); };
    void removeAsteroid(const std::string& id); 
    std::unordered_map<std::string, std::unique_ptr<Asteroid>>& getAsteroids() { return asteroids; };
    
private:
    
    void spawnAsteroid(Vector2 playerPos);
    AsteroidManager();
    int asteroidCounter = 0;  // TODO: DOES NOT GET USED AS THE ASTEROID ITSELF HAS A COUNTER
};