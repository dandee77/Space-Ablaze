#include "GameTimer.hpp"
#include "ResourceManager.hpp"
#include <string>

GameTimer::GameTimer() 
    : startTime(0), isPaused(true), pausedTime(0), totalPausedDuration(0) {}

void GameTimer::start() {
    startTime = GetTime();
    isPaused = false;
    pausedTime = 0;
    totalPausedDuration = 0;
}

void GameTimer::pause() {
    if (!isPaused && startTime > 0) {
        isPaused = true;
        pausedTime = GetTime();
    }
}

void GameTimer::unpause() {
    if (isPaused && startTime > 0) {
        totalPausedDuration += GetTime() - pausedTime;
        isPaused = false;
        pausedTime = 0;
    }
}

void GameTimer::reset() {
    startTime = 0;
    isPaused = true;
    pausedTime = 0;
    totalPausedDuration = 0;
}

float GameTimer::getElapsedTime() const {
    if (startTime == 0) return 0.0f;

    if (isPaused) {
        return (pausedTime - startTime) - totalPausedDuration;
    } else {
        return (GetTime() - startTime) - totalPausedDuration;
    }
}

std::string GameTimer::getElapsedTimeString() const {
    int totalSeconds = static_cast<int>(getElapsedTime());
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;
    return std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds);
}

void GameTimer::draw(int screenWidth) {
    const int fontSize = 80;
    const float padding = 220.0f;

    Vector2 screenPos = {screenWidth / 2.0f, padding};

    std::string timeText = getElapsedTimeString();

    Font font = ResourceManager::GetInstance().GetFont("primary_font");
    Vector2 textSize = MeasureTextEx(font, timeText.c_str(), fontSize, 1.0f);

    screenPos.x -= textSize.x / 2.0f;

    Rectangle textRect = {
        screenPos.x - 10,
        screenPos.y - 5,
        textSize.x + 20,
        textSize.y + 10
    };

    DrawRectangleRounded(textRect, 0.3f, 5, Fade(BLACK, 0.6f));

    DrawTextEx(
        font,
        timeText.c_str(),
        (Vector2){screenPos.x, screenPos.y},
        fontSize,
        1.0f,
        WHITE
    );

    DrawRectangleRoundedLines(textRect, 0.3f, 5, Fade(WHITE, 0.8f));
}
