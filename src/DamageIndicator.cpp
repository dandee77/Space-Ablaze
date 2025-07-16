#include "DamageIndicator.hpp"
#include "ResourceManager.hpp"
#include <string>

DamageIndicator::DamageIndicator(Vector2 pos, int damage, int maxDmg)
    : position(pos), damageAmount(damage), maxDamage(maxDmg), lifetime(0.0f), maxLifetime(1.5f), finished(false)
{
    velocity = {
        (float)GetRandomValue(-10, 10),
        (float)GetRandomValue(-10, 10)
    };
    
    if (damage >= maxDmg) {
        color = RED;
        scale = 1.5f; 
    } else {
        color = WHITE;
        scale = 1.0f;
    }
    
    damageText = std::to_string(damage);
}

void DamageIndicator::update()
{
    if (finished) return;
    
    float deltaTime = GetFrameTime();
    lifetime += deltaTime;

    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
    
    velocity.x *= 0.98f;
    velocity.y *= 0.95f;
    
    float normalizedTime = lifetime / maxLifetime;
    if (normalizedTime < 0.3f) {
        float growFactor = 1.0f + (normalizedTime / 0.3f) * 0.3f;
        scale = (damageAmount >= maxDamage ? 1.2f : 1.0f) * growFactor;
    } else {
        float shrinkFactor = 1.3f - ((normalizedTime - 0.3f) / 0.7f) * 0.8f;
        scale = (damageAmount >= maxDamage ? 1.2f : 1.0f) * shrinkFactor;
    }
    
    float alpha = 1.0f - (lifetime / maxLifetime);
    color.a = (unsigned char)(255 * alpha);

    if (lifetime >= maxLifetime) {
        finished = true;
    }
}

void DamageIndicator::draw()
{
    if (finished) return;
    
    float fontSize = 4.0f * scale;
    Vector2 textSize = MeasureTextEx(ResourceManager::GetInstance().GetFont("primary_font"), damageText.c_str(), fontSize, 0);
    Vector2 textPos = {position.x - textSize.x / 2.0f, position.y - textSize.y / 2.0f};
    
    DrawTextEx(ResourceManager::GetInstance().GetFont("primary_font"), damageText.c_str(), textPos, fontSize, 0, color);
}
