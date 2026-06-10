#include "flock.hpp"

void Flock::add(Boid boid) {
    boids.push_back(boid);
}

void Flock::update(const std::vector<Predator>& predators, const Settings& settings) {
    for (int i = 0; i < boids.size(); i++) {
        boids[i].update(boids, predators, settings);
    }
}

void Flock::draw() {
    for (int i = 0; i < boids.size(); i++) {
        boids[i].draw();
    }
}
