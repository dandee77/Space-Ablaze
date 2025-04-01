#pragma once
#include "raylib.h"
#include "Animator.hpp"
#include "ResourceManager.hpp"
#include "Scene.hpp"
#include <map>
#include <memory>
#include <string>


class Application
{
private:

    Camera2D m_camera;
    RenderTexture2D m_target;
    std::map<std::string, std::shared_ptr<Scene>> m_sceneFactory;
    std::string m_currentScene;
    
public:

    Application();
    void Run();
    ~Application();
};