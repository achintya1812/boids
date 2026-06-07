#include "boid.hpp"
#include "raymath.h"

Boid::Boid(float x, float y) {
    position = {x, y};
    
    velocity = { (float)GetRandomValue(-10, 10), (float)GetRandomValue(-10, 10)};
    velocity = Vector2Normalize(velocity);
}

void Boid::update(const std::vector<Boid>& flock) {
    float protected_range = 0.0f;
    float visual_range = 0.0f;

    Vector2 v1 = separation(flock, protected_range);
    Vector2 v2 = alignment(flock, visual_range);
    Vector2 v3 = cohesion(flock, visual_range);

    velocity = Vector2Add(velocity, v1);
    velocity = Vector2Add(velocity, v2);
    velocity = Vector2Add(velocity, v3);
    
    position = Vector2Add(position, velocity);
}

void Boid::draw() {
    DrawCircleV(position, 5.0f, DARKBLUE);
}   

Vector2 Boid::separation(const std::vector<Boid>& flock, float protected_range) {
    Vector2 c = {0.0f, 0.0f};

    for (int i = 0; i < flock.size(); i++) {
        if (&flock[i] != this) {
            float distance = Vector2Distance(position, flock[i].position);

            if (distance < protected_range) {
                Vector2 difference = Vector2Subtract(flock[i].position, position);
                c = Vector2Subtract(c, difference);
            }
        }
    }

    return c;
}

Vector2 Boid::alignment(const std::vector<Boid>& flock, float visual_range) {
    Vector2 percieved_velocity = {0.0f, 0.0f};
    int neighbour_count = 0;

    for (int i = 0; i < flock.size(); i++) {
        if (&flock[i] != this) {
            float distance = Vector2Distance(position, flock[i].position);

            if (distance < visual_range) {
                percieved_velocity = Vector2Add(percieved_velocity, flock[i].velocity);
                neighbour_count++;
            }
        }
    }

    if (neighbour_count > 0) {
        percieved_velocity.x /= neighbour_count;
        percieved_velocity.y /= neighbour_count;

        return {Vector2Subtract(percieved_velocity, velocity).x / 8.0f, Vector2Subtract(percieved_velocity, velocity).y / 8.0f};
    }

    return {0.0f, 0.0f};

}

Vector2 Boid::cohesion(const std::vector<Boid>& flock, float visual_range) {
    Vector2 percieved_center = {0.0f, 0.0f};
    int neighbour_count = 0;

    for (int i = 0; i < flock.size(); i++) {
        if (&flock[i] != this) {
            float distance = Vector2Distance(position, flock[i].position);

            if (distance < visual_range) {
                percieved_center = Vector2Add(percieved_center, flock[i].position);
                neighbour_count++;
            }
        }
    }

    if (neighbour_count > 0) {
        percieved_center.x /= neighbour_count;
        percieved_center.y /= neighbour_count;
        
        return {Vector2Subtract(percieved_center, velocity).x / 100.0f, Vector2Subtract(percieved_center, velocity).y / 100.0f};
    }

    return {0.0f, 0.0f};
}