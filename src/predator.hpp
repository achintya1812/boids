#pragma once 
#include "raylib.h"
#include "boid.hpp"
#include <vector>

class Predator {
    public:
        Vector2 position;
        Vector2 velocity;

        Predator(float x, float y);

        void update(const std::vector<Boid>& flock);
        void draw();
};

