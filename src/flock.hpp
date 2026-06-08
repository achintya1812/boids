#pragma once 
#include "boid.hpp"
#include <vector>

class Flock {
    public:
        std::vector<Boid> boids;

        void add(Boid boid);
        void update(Vector2 predator_position);
        void draw();
};
