#pragma once
#include "raylib.h"


class Entity {

public:
    Entity(Rectangle rect);
    bool update(); // boolean to check if the entity wants to shoot bullets
    void draw() const;

private:
    Rectangle m_rect;
    Color m_color = WHITE;
    float m_speed = 0.0f;
    float m_rotation = 0.0f;
    Vector2 m_velocity = {0.0f, 0.0f};
    Texture2D m_texture = {0};
    bool m_isVisible = true;
    bool m_isActive = true;
};