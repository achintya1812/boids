#pragma once 
#include "raylib.h"
#include "boid.hpp"
#include "settings.hpp"
#include <vector>

class Predator {
    public:
        Vector2 position;
        Vector2 velocity;

        Predator(float x, float y);

        void update(const std::vector<Boid>& flock, const Settings& settings);
        void draw();
};

