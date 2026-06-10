#include "raylib.h"
#include "imgui.h"
#include "rlImGui.h"

#include "boid.hpp"
#include "predator.hpp"
#include "flock.hpp"
#include "settings.hpp"

int main() {
    const int screen_width = 1000;
    const int screen_height = 1000;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screen_width, screen_height, "boids");
    SetTargetFPS(60);
    rlImGuiSetup(true);

    Settings settings;

    Flock flock;
    std::vector<Predator> predators;
    predators.push_back(Predator(screen_width / 2.0f, screen_height / 2.0f));
    static const std::vector<Predator> no_predators;   

    for (int i = 0; i < settings.swarm_size; i++) {
        float initial_x = (float)GetRandomValue(50, screen_width - 50);
        float initial_y = (float)GetRandomValue(50, screen_height - 50);

        flock.add(Boid(initial_x, initial_y));
    }

    while (!WindowShouldClose()) {
        while (flock.boids.size() < static_cast<size_t>(settings.swarm_size)) {
            float x = static_cast<float>(GetRandomValue(50, screen_width - 50));
            float y = static_cast<float>(GetRandomValue(50, screen_height - 50));

            flock.add(Boid(x, y));
        }

        while (flock.boids.size() > static_cast<size_t>(settings.swarm_size)) {
            flock.boids.pop_back();
        }

        while (predators.size() < static_cast<size_t>(settings.predator_count)) {
            float x = (float)GetRandomValue(50, screen_width - 50);
            float y = (float)GetRandomValue(50, screen_height - 50);

            predators.push_back(Predator(x, y));
        }

        while (predators.size() > static_cast<size_t>(settings.predator_count)) {
            predators.pop_back();
        }

        if (settings.predators_enabled) {
            for (int i = 0; i < predators.size(); i++) {
                predators[i].update(flock.boids, settings);
            }

            flock.update(predators, settings);
        }

        else {
            flock.update(no_predators, settings);
        }

        BeginDrawing();
            ClearBackground(BLACK);

            flock.draw();
            
            if (settings.predators_enabled) {
                for (int i = 0; i < predators.size(); i++) {
                    predators[i].draw();
                }
            }

            DrawFPS(10, 10);

            rlImGuiBegin();

            ImGui::Begin("Simulation Controls");

            ImGui::SliderInt("Swarm size", &settings.swarm_size, 100, 500);
            ImGui::Text("Active boids: %d", static_cast<int>(flock.boids.size()));

            ImGui::Checkbox("Predators enabled", &settings.predators_enabled);
            ImGui::SliderInt("Predator count", &settings.predator_count, 1, 10);

            ImGui::SliderFloat("Protected range", &settings.protected_range, 5.0f, 50.0f);
            ImGui::SliderFloat("Visual range", &settings.visual_range, 30.0f, 200.0f);

            if (settings.protected_range > settings.visual_range) {
                settings.protected_range = settings.visual_range;
            }
            ImGui::SliderFloat("Predator range", &settings.predator_range, 50.0f, 300.0f);

            ImGui::SliderFloat("Prey minimum speed", &settings.prey_min_speed, 0.5f, 5.0f);
            ImGui::SliderFloat("Prey maximum speed", &settings.prey_max_speed, 2.0f, 10.0f);

            ImGui::SliderFloat("Predator minimum speed", &settings.predator_min_speed, 0.5f, 6.0f);
            ImGui::SliderFloat("Predator maximum speed", &settings.predator_max_speed, 2.0f, 12.0f);

            ImGui::End();

            rlImGuiEnd();
        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();
    return 0;
}
