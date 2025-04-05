#pragma once
#include "Entity.hpp"
#include "raylib.h"


class Player : public Entity
{
public:
    Player();
    void init();
    bool update() override;
    void draw() override;

private:
    bool playerAccelerating;
};