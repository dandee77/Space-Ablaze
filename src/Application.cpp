#include "Application.hpp"
#include "Animation.hpp"
#include "raymath.h"
#include <memory>
#include "ResourceManager.hpp"

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
}

Application::~Application()
{
    UnloadRenderTexture(m_target);
    CloseWindow();
}

void Application::Run()
{
    ResourceManager& resourceManager = ResourceManager::GetInstance();
    resourceManager.LoadTextures();
    while (resourceManager.GetTextureLoadingProgress() < 100)
    {
        resourceManager.ProcessLoadedTextures();
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("Loading...", 10, 10, 20, WHITE);
        DrawText(TextFormat("Loading progress: %d%%", resourceManager.GetTextureLoadingProgress()), 10, 40, 20, WHITE);
        EndDrawing();
    }
    
    resourceManager.ProcessLoadedTextures();

    int resolutionLoc = GetShaderLocation(resourceManager.GetShader("shader"), "resolution");
    RenderTexture targetFinal = LoadRenderTexture(screenWidth, screenHeight);
    int timeLoc = GetShaderLocation(resourceManager.GetShader("shader"), "time");

    Texture2D bg = resourceManager.GetTexture("game_background");
    auto bgAnim = std::make_shared<Animation>(bg, bg.width / 3, bg.height / 3, 0.1f, true);
    m_animator.AddAnimation("bg", bgAnim);
    m_animator.Play("bg");

    while (!WindowShouldClose())
    {
        float time = GetTime();
        float resolution[2] = { (float)screenWidth, (float)screenHeight };
        SetShaderValue(resourceManager.GetShader("shader"), resolutionLoc, resolution, SHADER_UNIFORM_VEC2);
        SetShaderValue(resourceManager.GetShader("shader"), timeLoc, &time, SHADER_UNIFORM_FLOAT);

        float scale = MIN((float)GetScreenWidth()/screenWidth, (float)GetScreenHeight()/screenHeight);
        float renderWidth = screenWidth * scale;
        float renderHeight = screenHeight * scale;
        float offsetX = (GetScreenWidth() - renderWidth) * 0.5f;
        float offsetY = (GetScreenHeight() - renderHeight) * 0.5f;

        BeginTextureMode(m_target);
            ClearBackground(BLUE);
            m_animator.Update();
            m_animator.Draw(Rectangle{0, 0, (float)screenWidth, (float)screenHeight}, false, WHITE);
        EndTextureMode();

        BeginTextureMode(targetFinal);
            ClearBackground(BLANK);
            BeginShaderMode(resourceManager.GetShader("shader"));
                DrawTextureRec(m_target.texture, 
                             Rectangle{0, 0, (float)m_target.texture.width, (float)-m_target.texture.height},
                             Vector2{0, 0}, 
                             WHITE);
            EndShaderMode();
        EndTextureMode();

        BeginDrawing();
            ClearBackground(BLACK);
            DrawTexturePro(targetFinal.texture,
                         Rectangle{0, 0, (float)screenWidth, (float)-screenHeight},
                         Rectangle{offsetX, offsetY, renderWidth, renderHeight},
                         Vector2{0, 0},
                         0.0f,
                         WHITE);
        EndDrawing();
    }

    UnloadRenderTexture(targetFinal);
}
