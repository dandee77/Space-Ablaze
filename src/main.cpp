#include "raylib.h"

int main()
{
    InitWindow(800, 600, "raylib [core] example - basic window");

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second

    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update

        // Draw
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
        DrawText("Press <ESC> to close me.", 240, 250, 20, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow(); // Close window and OpenGL context
}