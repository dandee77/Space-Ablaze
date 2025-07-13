#include "Timer.hpp"
#include <algorithm>
#include <string>

Cooldown::Cooldown() 
    : cooldownDuration(0), lastUsedTime(0), isPaused(false), saveTime(0), pauseDuration(0) {}

Cooldown::Cooldown(float cooldown) 
    : cooldownDuration(cooldown), lastUsedTime(0), isPaused(false), saveTime(0), pauseDuration(0) {}

bool Cooldown::isOnCooldown() const 
{
    float currentTime = GetTime();
    return (currentTime - lastUsedTime) < cooldownDuration;
}

void Cooldown::startCooldown() 
{
    lastUsedTime = GetTime();
}

float Cooldown::remainingCooldown() const 
{
    float currentTime = isPaused ? saveTime : (GetTime() - pauseDuration);
    float elapsedTime = currentTime - lastUsedTime;
    return std::max(0.0f, cooldownDuration - elapsedTime);
}

std::string Cooldown::remainingCooldownString() const 
{
    int remainingSeconds = static_cast<int>(remainingCooldown());
    int minutes = remainingSeconds / 60;
    int seconds = remainingSeconds % 60;
    return std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds);
}

void Cooldown::updateCooldownDuration(float newCooldownDuration) 
{
    cooldownDuration = newCooldownDuration;
}

void Cooldown::pause() 
{
    isPaused = true;
    saveTime = GetTime() - pauseDuration;
}

void Cooldown::unpause() 
{
    isPaused = false;
    pauseDuration = GetTime() - saveTime;
}