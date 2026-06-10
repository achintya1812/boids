#pragma once 
#include "boid.hpp"
#include "settings.hpp"
#include <vector>

class Predator;
class Flock {
    public:
        std::vector<Boid> boids;

        void add(Boid boid);
        void update(const std::vector<Predator>& predators, const Settings& settings);
        void draw();
};
