#include "KillCounter.hpp"
#include <cmath>

KillCounter::KillCounter() 
    : count(0), tilt(0.0f), scale(1.0f), cooldown(0.3f) {}

void KillCounter::increment() {
    count++;
    cooldown.startCooldown();
    updateAnimation();
}

void KillCounter::update() {
    updateAnimation();
}

void KillCounter::updateAnimation() {
    if (cooldown.isOnCooldown()) {
        float progress = 1.0f - (cooldown.remainingCooldown() / cooldown.getCooldownDuration());
        tilt = sinf(progress * PI * 4) * 15.0f * (1.0f - progress);
        scale = 1.0f + sinf(progress * PI) * 0.3f;
    } else {
        tilt = 0.0f;
        scale = 1.0f;
    }
}

void KillCounter::draw() {
    const int fontSize = 120;
    const float padding = 220.0f;
    Vector2 screenPos = {padding, padding};

    std::string text = "X" + std::to_string(count);
    Font font = ResourceManager::GetInstance().GetFont("primary_font");
    Vector2 textSize = MeasureTextEx(font, text.c_str(), fontSize, 1.0f);

    Rectangle textRect = {
        screenPos.x,
        screenPos.y,
        textSize.x,
        textSize.y
    };

    DrawRectangleRounded(
        {textRect.x - 10, textRect.y - 5, textRect.width + 20, textRect.height + 10},
        0.5f, 5, Fade(BLACK, 0.7f)
    );

    DrawTextPro(
        font,
        text.c_str(),
        {textRect.x + textRect.width / 2, textRect.y + textRect.height / 2},
        {textRect.width / 2, textRect.height / 2},
        tilt,
        fontSize,
        2.0f,
        {255, 50, 50, 255}
    );

    DrawRectangleRoundedLines(
        {textRect.x - 10, textRect.y - 5, textRect.width + 20, textRect.height + 10},
        0.5f, 5, RED
    );
}
