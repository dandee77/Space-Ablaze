#pragma once
#include "raylib.h"
#include "Animator.hpp"
#include "ResourceManager.hpp"


class Application
{
private:

    Camera2D m_camera;
    RenderTexture2D m_target;
    Shader m_shader;
    Animator m_animator; //TODO:  make it an singleton

public:

    Application();
    void Run();
    ~Application();
};