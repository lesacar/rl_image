#include "raylib.h"

int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "RL Image Project");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        
        ClearBackground(RAYWHITE);
        
        DrawText("Raylib C++ template", 190, 200, 20, DARKGRAY);
        
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}
