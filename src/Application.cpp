#include "Application.hpp"
#include "Animation.hpp"
#include "raymath.h"
#include <memory>
#include "ResourceManager.hpp"
#include <iostream>

// scnenes
#include "LoadingScreen.hpp"
#include "MainMenu.hpp"


#define MIN(a,b) (((a)<(b))?(a):(b))

static constexpr int screenWidth = 3840;
static constexpr int screenHeight = 2160;

Application::Application()
{
    const int windowWidth = 1280;
    const int windowHeight = 720;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT); 
    InitWindow(windowWidth, windowHeight, "Space Ablaze");
    SetTargetFPS(60);

    m_target = LoadRenderTexture(screenWidth, screenHeight);
    SetTextureFilter(m_target.texture, TEXTURE_FILTER_BILINEAR);

    m_camera = Camera2D{ 0 };
    m_camera.target = { 0.0f, 0.0f };
    m_camera.offset = { (float)screenWidth/2.0f, (float)screenHeight/2.0f };
    m_camera.rotation = 0.0f;
    m_camera.zoom = 1.0f;

    ResourceManager::GetInstance().LoadShaders(); 
    ResourceManager::GetInstance().LoadFonts();
    // ResourceManager::GetInstance().LoadMusics();
    // ResourceManager::GetInstance().LoadSounds();


    // Initialize the first scene
    m_currentScene = "LoadingScreen";
    m_sceneFactory = 
    {
        {"LoadingScreen", std::make_shared<LoadingScreen>()},
        {"MainMenu", std::make_shared<MainMenu>()}
    };

    m_sceneFactory[m_currentScene]->onSwitch();
}

Application::~Application()
{
    UnloadRenderTexture(m_target);
    CloseWindow();
}

void Application::Run()
{
    ResourceManager& resourceManager = ResourceManager::GetInstance();
    
    // Load shaders and get uniform locations
    Shader mainShader = resourceManager.GetShader("shader");
    Shader barrelShader = resourceManager.GetShader("barrel_distortion");
    
    int resolutionLocMain = GetShaderLocation(mainShader, "resolution");
    int timeLocMain = GetShaderLocation(mainShader, "time");
    int resolutionLocBarrel = GetShaderLocation(barrelShader, "resolution");
    
    // Create render textures
    RenderTexture targetFinal = LoadRenderTexture(screenWidth, screenHeight); //? with the shader applied
    RenderTexture borderTarget = LoadRenderTexture(screenWidth, screenHeight); //? the border without the shader applied
    RenderTexture borderTargetFinal = LoadRenderTexture(screenWidth, screenHeight); //? the border with the shader applied
    
    // Set texture filters
    SetTextureFilter(targetFinal.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(borderTarget.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(borderTargetFinal.texture, TEXTURE_FILTER_BILINEAR);

    while (!WindowShouldClose())
    {
        // Update timing and resolution values
        float time = GetTime();
        float resolution[2] = { (float)screenWidth, (float)screenHeight };
        
        // Set shader uniforms
        SetShaderValue(mainShader, resolutionLocMain, resolution, SHADER_UNIFORM_VEC2);
        SetShaderValue(mainShader, timeLocMain, &time, SHADER_UNIFORM_FLOAT);
        SetShaderValue(barrelShader, resolutionLocBarrel, resolution, SHADER_UNIFORM_VEC2);

        // Calculate scaling for different resolutions
        float scale = MIN((float)GetScreenWidth()/screenWidth, (float)GetScreenHeight()/screenHeight);
        float renderWidth = screenWidth * scale;
        float renderHeight = screenHeight * scale;
        float offsetX = (GetScreenWidth() - renderWidth) * 0.5f;
        float offsetY = (GetScreenHeight() - renderHeight) * 0.5f;

        // Render game content to main target
        BeginTextureMode(m_target);
            ClearBackground(BLACK);
            if (m_sceneFactory.find(m_currentScene) != m_sceneFactory.end())
            {
                std::string nextScene = m_sceneFactory[m_currentScene]->update();
                if (m_sceneFactory.find(nextScene) != m_sceneFactory.end())
                {
                    m_sceneFactory[m_currentScene]->onExit();
                    m_sceneFactory[nextScene]->onSwitch();
                }
                m_currentScene = nextScene;
            }
            m_sceneFactory[m_currentScene]->draw();
        EndTextureMode();

        // Apply main shader effects to game content
        BeginTextureMode(targetFinal);
            ClearBackground(BLANK);
            BeginShaderMode(mainShader);
                DrawTextureRec(m_target.texture, 
                             Rectangle{0, 0, (float)m_target.texture.width, (float)-m_target.texture.height},
                             Vector2{0, 0}, 
                             WHITE);
            EndShaderMode();
        EndTextureMode();

        // Render border to separate target
        BeginTextureMode(borderTarget);
            ClearBackground(BLANK);
            DrawTexturePro(resourceManager.GetTexture("border"),
                         Rectangle{0, 0, 
                                  (float)resourceManager.GetTexture("border").width, 
                                  (float)resourceManager.GetTexture("border").height},
                         Rectangle{0, 0, (float)screenWidth, (float)screenHeight},
                         Vector2{0, 0},
                         0.0f,
                         WHITE);
        EndTextureMode();

        // Apply barrel distortion to border
        BeginTextureMode(borderTargetFinal);
            ClearBackground(BLANK);
            BeginShaderMode(barrelShader);
                DrawTextureRec(borderTarget.texture, 
                             Rectangle{0, 0, (float)borderTarget.texture.width, (float)-borderTarget.texture.height},
                             Vector2{0, 0}, 
                             WHITE);
            EndShaderMode();
        EndTextureMode();

        // Final composition to screen
        BeginDrawing();
            ClearBackground(BLACK);
            
            // Draw game content
            DrawTexturePro(targetFinal.texture,
                         Rectangle{0, 0, (float)screenWidth, (float)-screenHeight},
                         Rectangle{offsetX, offsetY, renderWidth, renderHeight},
                         Vector2{0, 0},
                         0.0f,
                         WHITE);
            
            // Draw distorted border on top
            DrawTexturePro(borderTargetFinal.texture,
                         Rectangle{0, 0, (float)screenWidth, (float)-screenHeight},
                         Rectangle{offsetX, offsetY, renderWidth, renderHeight},
                         Vector2{0, 0},
                         0.0f,
                         WHITE);
            
            // Optional: Draw debug info
            // DrawFPS(10, 10);
        EndDrawing();
    }

    // Cleanup
    UnloadRenderTexture(targetFinal);
    UnloadRenderTexture(borderTarget);
    UnloadRenderTexture(borderTargetFinal);
}