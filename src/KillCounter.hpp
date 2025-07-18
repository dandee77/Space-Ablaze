#pragma once
#include "raylib.h"
#include "Timer.hpp"
#include "ResourceManager.hpp"
#include <string>

class KillCounter {
public:
    KillCounter();

    void increment();
    void update();
    void draw();
    void reset();
    int getCount() const { return count; }

private:
    int count;
    float tilt;
    float scale;
    Cooldown cooldown;

    void updateAnimation();
};
