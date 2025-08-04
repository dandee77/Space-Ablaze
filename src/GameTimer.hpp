#pragma once
#include "raylib.h"
#include <string>

class GameTimer
{
private:
    float startTime;
    bool isPaused;
    float pausedTime;
    float totalPausedDuration;

public:
    GameTimer();

    void start();
    void pause();
    void unpause();
    void reset();

    int setStartTime(float time);

    float getElapsedTime() const;
    std::string getElapsedTimeString() const;
    bool isRunning() const { return !isPaused && startTime > 0; }

    void draw(int screenWidth); 
};
