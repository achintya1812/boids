#include <raylib.h>
#include "boid.hpp"
#include <flock.hpp>

int main() {
    const int screen_width = 1000;
    const int screen_height = 1000;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screen_width, screen_height, "boids");
    SetTargetFPS(60);

    Flock flock;
    int swarm_size = 250;

    for (int i = 0; i < swarm_size; i++) {
        float initial_x = (float)GetRandomValue(50, screen_width - 50);
        float initial_y = (float)GetRandomValue(50, screen_height - 50);

        flock.add(Boid(initial_x, initial_y));
    }

    while (!WindowShouldClose()) {
        flock.update();

        BeginDrawing();
            ClearBackground(BLACK);

            flock.draw();

            DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}