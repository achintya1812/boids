#include "raylib.h"
#include <iostream>

int main() {
    InitWindow(800, 450, "My C++ Raylib Game");
    
    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("C++ is working!", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}