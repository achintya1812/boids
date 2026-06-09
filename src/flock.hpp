#pragma once 
#include "boid.hpp"
#include "settings.hpp"
#include <vector>

class Flock {
    public:
        std::vector<Boid> boids;

        void add(Boid boid);
        void update(Vector2 predator_position, const Settings& settings);
        void draw();
};
