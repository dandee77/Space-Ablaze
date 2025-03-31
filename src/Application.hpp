#pragma once
#include "raylib.h"
#include "Animator.hpp"


class Application
{
private:

    Camera2D m_camera;
    RenderTexture2D m_target;
    Shader m_shader;
    Animator m_animator;

public:

    Application();
    void Run();
    ~Application();
};