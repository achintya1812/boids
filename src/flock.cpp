#include "flock.hpp"

void Flock::add(Boid boid) {
    boids.push_back(boid);
}

void Flock::update(Vector2 predator_position, const Settings& settings) {
    for (int i = 0; i < boids.size(); i++) {
        boids[i].update(boids, predator_position, settings);
    }
}

void Flock::draw() {
    for (int i = 0; i < boids.size(); i++) {
        boids[i].draw();
    }
}
