#include "predator.hpp"
#include "boid.hpp"
#include "settings.hpp"
#include "raymath.h"

Predator::Predator(float x, float y) {
    position = {x, y};
    velocity = {0.0f, 0.0f};
}

void Predator::update(const std::vector<Boid>& flock, const Settings& settings) {
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
    float turn_force = 1.0f;
    if (position.x < margin) velocity.x += turn_force;
    if (position.x > GetScreenWidth() - margin) velocity.x -= turn_force;
    if (position.y < margin) velocity.y += turn_force;
    if (position.y > GetScreenHeight() - margin) velocity.y -= turn_force;

    float speed = Vector2Length(velocity);
    
    if (speed > 0.0f) {
        if (speed > settings.predator_max_speed) {
            velocity.x = (velocity.x / speed) * settings.predator_max_speed;
            velocity.y = (velocity.y / speed) * settings.predator_max_speed;
        } else if (speed < settings.predator_min_speed) {
            velocity.x = (velocity.x / speed) * settings.predator_min_speed;
            velocity.y = (velocity.y / speed) * settings.predator_min_speed;
        }
    }
    
    position = Vector2Add(position, velocity);
}

void Predator::draw() {
    const float side_length = 35.0f;

    const float r = side_length / std::sqrt(3.0f);

    float theta = std::atan2f(velocity.y, velocity.x);
    
    Vector2 vertex1 = {position.x + r * std::cosf(theta), position.y + r * std::sinf(theta)};
    Vector2 vertex2 = {position.x + r * std::cosf(theta + 2.0944f), position.y + r * std::sinf(theta + 2.0944f)};
    Vector2 vertex3 = {position.x + r * std::cosf(theta + 4.1888f), position.y + r * std::sinf(theta + 4.1888f)};

    DrawTriangle(vertex1, vertex3, vertex2, RED);
}