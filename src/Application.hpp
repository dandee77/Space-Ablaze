#pragma once
#include "raylib.h"


class Application
{
private:

    Camera2D m_camera;
    RenderTexture2D m_target;
    Shader m_shader;

public:

    Application();
    void Run();
    ~Application();
};