#pragma once 
#include "boid.hpp"
#include <vector>

class Flock {
    public:
        std::vector<Boid> boids;

        void add(Boid boid);
        void update();
        void draw();
};