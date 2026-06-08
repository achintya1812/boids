#include "predator.hpp"
#include "boid.hpp"
#include "raymath.h"

Predator::Predator(float x, float y) {
    position = {x, y};
    velocity = {0.0f, 0.0f};
}

void Predator::update(const std::vector<Boid>& flock) {
    if (flock.empty()) {
        return;
    }

    float closest_distance = 1e9;
    Vector2 target_position = position;

    for (int i = 0; i < flock.size(); i++) {
        float distance = Vector2Distance(position, flock[i].position);
        if (distance < closest_distance) {
            closest_distance = distance;
            target_position = flock[i].position;
        }
    }

    Vector2 desired = Vector2Subtract(target_position, position);
    desired = Vector2Normalize(desired);
    desired = Vector2Scale(desired, 0.2f); 
    velocity = Vector2Add(velocity, desired);

    float margin = 50.0f;
    float turnForce = 1.0f;
    if (position.x < margin) velocity.x += turnForce;
    if (position.x > GetScreenWidth() - margin) velocity.x -= turnForce;
    if (position.y < margin) velocity.y += turnForce;
    if (position.y > GetScreenHeight() - margin) velocity.y -= turnForce;

    float speed = Vector2Length(velocity);
    float max_speed = 5.0f;
    float min_speed = 2.0f; 
    
    if (speed > 0.0f) {
        if (speed > max_speed) {
            velocity.x = (velocity.x / speed) * max_speed;
            velocity.y = (velocity.y / speed) * max_speed;
        } else if (speed < min_speed) {
            velocity.x = (velocity.x / speed) * min_speed;
            velocity.y = (velocity.y / speed) * min_speed;
        }
    }
    
    position = Vector2Add(position, velocity);
}

void Predator::draw() {
    DrawCircleV(position, 15.0f, RED);
}