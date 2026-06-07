#pragma once
#include "raylib.h"
#include <vector>

class Boid {
    public:
        Vector2 position;
        Vector2 velocity;

        Color color; 

        Boid(float x, float y);

        void update(const std::vector<Boid>& flock);
        void draw();

    private:
        Vector2 separation(const std::vector<Boid>& flock, float protected_range);
        Vector2 alignment(const std::vector<Boid>& flock, float visual_range);
        Vector2 cohesion(const std::vector<Boid>& flock, float visual);
};

