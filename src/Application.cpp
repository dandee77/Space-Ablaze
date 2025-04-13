#include "Application.hpp"
#include "Animation.hpp"
#include "raymath.h"
#include <memory>
#include "ResourceManager.hpp"
#include <iostream>

// scnenes
#include "LoadingScreen.hpp"
#include "MainMenu.hpp"
#include "Game.hpp"


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
        {"MainMenu", std::make_shared<MainMenu>()},
        {"Game", std::make_shared<Game>()}
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
    
  
    Shader mainShader = resourceManager.GetShader("shader");
    Shader barrelShader = resourceManager.GetShader("barrel_distortion");
    
    int resolutionLocMain = GetShaderLocation(mainShader, "resolution");
    int timeLocMain = GetShaderLocation(mainShader, "time");
    int resolutionLocBarrel = GetShaderLocation(barrelShader, "resolution");
  
    RenderTexture targetFinal = LoadRenderTexture(screenWidth, screenHeight); //? with the shader applied
    RenderTexture borderTarget = LoadRenderTexture(screenWidth, screenHeight); //? the border without the shader applied
    RenderTexture borderTargetFinal = LoadRenderTexture(screenWidth, screenHeight); //? the border with the shader applied
    
  
    SetTextureFilter(targetFinal.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(borderTarget.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(borderTargetFinal.texture, TEXTURE_FILTER_BILINEAR);

        while (!WindowShouldClose())
    {

        float time = GetTime();
        float resolution[2] = { (float)screenWidth, (float)screenHeight };
        
   
        SetShaderValue(mainShader, resolutionLocMain, resolution, SHADER_UNIFORM_VEC2);
        SetShaderValue(mainShader, timeLocMain, &time, SHADER_UNIFORM_FLOAT);
        SetShaderValue(barrelShader, resolutionLocBarrel, resolution, SHADER_UNIFORM_VEC2);

     
        float scale = MIN((float)GetScreenWidth()/screenWidth, (float)GetScreenHeight()/screenHeight);
        // ? Set the mouse offset and scale
        Vector2 mouse = GetMousePosition();
        Vector2 virtualMouse = { 0 };
        virtualMouse.x = (mouse.x - ((float)GetScreenWidth() - ((float)screenWidth*scale))*0.5f)/scale;
        virtualMouse.y = (mouse.y - ((float)GetScreenHeight() - ((float)screenHeight*scale))*0.5f)/scale;
        virtualMouse = Vector2Clamp(virtualMouse, Vector2Zero(), Vector2{ (float)screenWidth, (float)screenHeight });
        SetMouseOffset(static_cast<int>(-((float)GetScreenWidth() - ((float)screenWidth*scale))*0.5f),
                    static_cast<int>(-((float)GetScreenHeight() - ((float)screenHeight*scale))*0.5f));
        SetMouseScale(1/scale, 1/scale);
        // ? Set the camera offset and scale
        float renderWidth = screenWidth * scale;
        float renderHeight = screenHeight * scale;
        float offsetX = (GetScreenWidth() - renderWidth) * 0.5f;
        float offsetY = (GetScreenHeight() - renderHeight) * 0.5f;

        BeginTextureMode(m_target);
            ClearBackground(BLACK);
            if (m_sceneFactory.find(m_currentScene) != m_sceneFactory.end())
            {
                std::string nextScene = m_sceneFactory[m_currentScene]->update();
                if (m_currentScene != nextScene)
                {
                    if (m_sceneFactory.find(nextScene) != m_sceneFactory.end())
                    {
                        m_sceneFactory[m_currentScene]->onExit();
                        m_sceneFactory[nextScene]->onSwitch();
                    }
                    else
                    {
                        std::cerr << "ERROR: Scene not found: " << nextScene << std::endl;
                        m_sceneFactory[m_currentScene]->onExit();
                        m_sceneFactory["MainMenu"]->onSwitch();
                        nextScene = "MainMenu";
                    }
                }
                m_currentScene = nextScene;
            }
            m_sceneFactory[m_currentScene]->draw();
        EndTextureMode();

    
        BeginTextureMode(targetFinal);
            ClearBackground(BLANK);
            BeginShaderMode(mainShader);
                DrawTextureRec(m_target.texture, 
                             Rectangle{0, 0, (float)m_target.texture.width, (float)-m_target.texture.height},
                             Vector2{0, 0}, 
                             WHITE);
            EndShaderMode();
        EndTextureMode();

    
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

        BeginTextureMode(borderTargetFinal);
            ClearBackground(BLANK);
            BeginShaderMode(barrelShader);
                DrawTextureRec(borderTarget.texture, 
                             Rectangle{0, 0, (float)borderTarget.texture.width, (float)-borderTarget.texture.height},
                             Vector2{0, 0}, 
                             WHITE);
            EndShaderMode();
        EndTextureMode();

   
        BeginDrawing();
            ClearBackground(BLACK);
            
            DrawTexturePro(targetFinal.texture,
                Rectangle{0, 0, (float)screenWidth, (float)-screenHeight},
                // Rectangle{offsetX + 40, offsetY + 35, renderWidth - 80, renderHeight - 70},
                Rectangle{offsetX, offsetY, renderWidth, renderHeight},
                Vector2{0, 0},
                0.0f,
                WHITE);
                
                
            DrawTexturePro(borderTargetFinal.texture,
                Rectangle{0, 0, (float)screenWidth, (float)-screenHeight},
                Rectangle{offsetX, offsetY, renderWidth, renderHeight},
                Vector2{0, 0},
                0.0f,
                WHITE);
                    
            DrawFPS(10, 10);
            
        EndDrawing();
    }


    UnloadRenderTexture(targetFinal);
    UnloadRenderTexture(borderTarget);
    UnloadRenderTexture(borderTargetFinal);
}