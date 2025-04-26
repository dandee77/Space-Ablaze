#pragma once

#include "raylib.h"
#include <string>

class Cooldown 
{
private:
    float cooldownDuration;
    float lastUsedTime;

public:
    bool isPaused;
    float saveTime;
    float pauseDuration;

    Cooldown();
    Cooldown(float cooldown);

    bool isOnCooldown() const;
    void startCooldown();
    float remainingCooldown() const;
    float getCooldownDuration() const { return cooldownDuration; }
    std::string remainingCooldownString() const;
    void updateCooldownDuration(float newCooldownDuration);
    void pause();
    void unpause();
};
