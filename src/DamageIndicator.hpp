#pragma once
#include "raylib.h"
#include <string>
#include "ResourceManager.hpp"

class DamageIndicator
{
public:
    DamageIndicator(Vector2 position, int damageAmount, int maxDamage);
    void update();
    void draw();
    bool isFinished() const { return finished; }

private:
    Vector2 position;
    Vector2 velocity;
    int damageAmount;
    int maxDamage;
    float lifetime;
    float maxLifetime;
    float scale;
    Color color;
    bool finished;
    std::string damageText;
};
