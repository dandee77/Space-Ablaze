#pragma once

#include "raylib.h"
#include <string>


enum ButtonType
{
    BUTTON_LEFT_SIDE = 0,
    BUTTON_CENTERED
};


class Button
{
public:

    Button(Rectangle rect, std::string text, float size, ButtonType type);

    void setColor(Color color);
    bool isHovered() const;
    bool isClicked() const;
    void draw();

private:
    Rectangle m_rect;
    std::string m_text;
    float m_size;
    ButtonType m_type;
    Color m_color;
};