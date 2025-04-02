#include "Button.hpp"
#include "ResourceManager.hpp"


Button::Button(Rectangle rect, std::string text, float size, ButtonType type)
    : m_rect(rect), m_text(text), m_size(size), m_type(type), m_color(Fade(SKYBLUE, 0.5f)) {}



void Button::setColor(Color color) {
    m_color = color;
}

// TODO: Adjusts buttons hitbox to be more accurate
bool Button::isHovered() const {
    return CheckCollisionPointRec(GetMousePosition(), m_rect);
}

bool Button::isClicked() const {
    return isHovered() && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

void Button::draw()
{
    DrawRectangleRec(m_rect, (isHovered()) ? Fade(m_color, 0.5f) : BLANK);

    switch(m_type)
    {
    case BUTTON_LEFT_SIDE:

        DrawTextEx(ResourceManager::GetInstance().GetFont("secondary_font"), m_text.c_str(), (Vector2){ m_rect.x + 10, m_rect.y }, m_size, 1.0f, ((isHovered()) ? BLACK : m_color));
        break;

    case BUTTON_CENTERED:

        Vector2 text_dimensions = MeasureTextEx(ResourceManager::GetInstance().GetFont("secondary_font"), m_text.c_str(), m_size, 0);
        float text_x = m_rect.x + (m_rect.width - text_dimensions.x) / 2;
        float text_y = m_rect.y + (m_rect.height - text_dimensions.y) / 2;
        DrawTextEx(ResourceManager::GetInstance().GetFont("secondary_font"), m_text.c_str(), (Vector2){ text_x, text_y }, m_size, 1.0f, ((isHovered()) ? BLACK : m_color));
        break;
    }
}