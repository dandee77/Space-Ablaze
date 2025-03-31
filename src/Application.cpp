#include "Application.hpp"
#include "raymath.h"

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

    // Resource Manager
}


Application::~Application()
{
    UnloadRenderTexture(m_target);
    CloseWindow();
}


void Application::Run()
{
    while (!WindowShouldClose())
    {
        float scale = MIN((float)GetScreenWidth()/screenWidth, (float)GetScreenHeight()/screenHeight);
        Vector2 mouse = GetMousePosition();
        Vector2 virtualMouse = { 0 };
        virtualMouse.x = (mouse.x - ((float)GetScreenWidth() - ((float)screenWidth*scale))*0.5f)/scale;
        virtualMouse.y = (mouse.y - ((float)GetScreenHeight() - ((float)screenHeight*scale))*0.5f)/scale;
        virtualMouse = Vector2Clamp(virtualMouse, Vector2Zero(), Vector2{ (float)screenWidth, (float)screenHeight });

        SetMouseOffset(static_cast<int>(-((float)GetScreenWidth() - ((float)screenWidth*scale))*0.5f),
                    static_cast<int>(-((float)GetScreenHeight() - ((float)screenHeight*scale))*0.5f));
        SetMouseScale(1/scale, 1/scale);


        BeginTextureMode(m_target);

            ClearBackground(BLACK); 

            // Update

            DrawText("Hello World", 100, 100, 120, WHITE);

        EndTextureMode();


        BeginDrawing();

            ClearBackground(BLACK); 

            DrawTexturePro(m_target.texture, Rectangle{ 0.0f, 0.0f, (float)m_target.texture.width, (float)-m_target.texture.height }, 
                        Rectangle{((float)GetScreenWidth() - ((float)screenWidth*scale))*0.5f,
                                    ((float)GetScreenHeight() - ((float)screenHeight*scale))*0.5f,
                                    (float)screenWidth*scale, (float)screenHeight*scale},
                                    Vector2Zero(), 0.0f, WHITE);
    
        EndDrawing();
    }
}